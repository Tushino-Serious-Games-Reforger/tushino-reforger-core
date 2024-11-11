import fire
import numpy as np
import uuid


class ProjectorStack:
    """
    Stack of points that are shifted / projected to put first one at origin.
    """
    def __init__(self, vec):
        self.vs = np.array(vec)
        
    def push(self, v):
        if len(self.vs) == 0:
            self.vs = np.array([v])
        else:
            self.vs = np.append(self.vs, [v], axis=0)
        return self
    
    def pop(self):
        if len(self.vs) > 0:
            ret, self.vs = self.vs[-1], self.vs[:-1]
            return ret
    
    def __mul__(self, v):
        s = np.zeros(len(v))
        for vi in self.vs:
            s = s + vi * np.dot(vi, v)
        return s


class GaertnerBoundary:
    """
        GärtnerBoundary

    See the passage regarding M_B in Section 4 of Gärtner's paper.
    """
    def __init__(self, pts):
        self.projector = ProjectorStack([])
        self.centers, self.square_radii = np.array([]), np.array([])
        self.empty_center = np.array([np.nan for _ in pts[0]])


def push_if_stable(bound, pt):
    if len(bound.centers) == 0:
        bound.square_radii = np.append(bound.square_radii, 0.0)
        bound.centers = np.array([pt])
        return True
    q0, center = bound.centers[0], bound.centers[-1]
    C, r2  = center - q0, bound.square_radii[-1]
    Qm, M = pt - q0, bound.projector
    Qm_bar = M * Qm
    residue, e = Qm - Qm_bar, sqdist(Qm, C) - r2
    z, tol = 2 * sqnorm(residue), np.finfo(float).eps * max(r2, 1.0)
    isstable = np.abs(z) > tol
    if isstable:
        center_new  = center + (e / z) * residue
        r2new = r2 + (e * e) / (2 * z)
        bound.projector.push(residue / np.linalg.norm(residue))
        bound.centers = np.append(bound.centers, np.array([center_new]), axis=0)
        bound.square_radii = np.append(bound.square_radii, r2new)
    return isstable


def pop(bound):
    n = len(bound.centers)
    bound.centers = bound.centers[:-1]
    bound.square_radii = bound.square_radii[:-1]
    if n >= 2:
        bound.projector.pop()
    return bound


class NSphere:
    def __init__(self, c, sqr):
        self.center = np.array(c)
        self.sqradius = sqr


def isinside(pt, nsphere, atol=1e-6, rtol=0.0):
    r2, R2 = sqdist(pt, nsphere.center), nsphere.sqradius
    return r2 <= R2 or np.isclose(r2, R2, atol=atol**2,rtol=rtol**2)


def allinside(pts, nsphere, atol=1e-6, rtol=0.0):
    for p in pts:
        if not isinside(p, nsphere, atol, rtol):
            return False
    return True


def move_to_front(pts, i):
    pt = pts[i]
    for j in range(len(pts)):
        pts[j], pt = pt, np.array(pts[j])
        if j == i:
            break
    return pts


def dist(p1, p2):
    return np.linalg.norm(p1 - p2)


def sqdist(p1, p2):
    return sqnorm(p1 - p2)


def sqnorm(p):
    return np.sum(np.array([x * x for x in p]))


def ismaxlength(bound):
    len(bound.centers) == len(bound.empty_center) + 1


def makeNSphere(bound):
    if len(bound.centers) == 0: 
        return NSphere(bound.empty_center, 0.0)
    return NSphere(bound.centers[-1], bound.square_radii[-1])


def _welzl(pts, pos, bdry):
    support_count, nsphere = 0, makeNSphere(bdry)
    if ismaxlength(bdry):
        return nsphere, 0
    for i in range(pos):
        if not isinside(pts[i], nsphere):
            isstable = push_if_stable(bdry, pts[i])
            if isstable:
                nsphere, s = _welzl(pts, i, bdry)
                pop(bdry)
                move_to_front(pts, i)
                support_count = s + 1
    return nsphere, support_count


def find_max_excess(nsphere, pts, k1):
    err_max, k_max = -np.inf, k1 - 1
    for (k, pt) in enumerate(pts[k_max:]):
        err = sqdist(pt, nsphere.center) - nsphere.sqradius
        if  err > err_max:
            err_max, k_max = err, k + k1
    return err_max, k_max - 1


def welzl(points, maxiterations=2000):
    pts, eps = np.array(points, copy=True), np.finfo(float).eps
    bdry, t = GaertnerBoundary(pts), 1
    nsphere, s = _welzl(pts, t, bdry)
    for i in range(maxiterations):
        e, k = find_max_excess(nsphere, pts, t + 1)
        if e <= eps:
            break
        pt = pts[k]
        push_if_stable(bdry, pt)
        nsphere_new, s_new = _welzl(pts, s, bdry)
        pop(bdry)
        move_to_front(pts, k)
        nsphere = nsphere_new
        t, s = s + 1, s_new + 1
    return nsphere


def sample_points_on_circle(center, radius, interval=0.1):
    x0, y0 = center
    t_values = np.arange(0, 2 * np.pi, interval)
   
    x_points = x0 + radius * np.cos(t_values)
    y_points = y0 + radius * np.sin(t_values)
   
    points = np.column_stack((x_points, y_points))
    return points


def parse_points(input_polygon_file):
    points = []
    with open(input_polygon_file, "rt") as f:
        for line in f:
            if "Position" in line:
                _, x, z, y = line.split()
                points.append([float(x), float(y)])
    return np.asarray(points)


def write_polygon(points, output_polygon_file):
    with open(output_polygon_file, "wt") as f:
        f.write("Points {\n")
        for i in range(len(points)):
            f.write(f'   ShapePoint "{random_guid()}" {{\n')
            f.write(f"      Position {points[i, 0]} 0.0 {points[i, 1]}\n")
            f.write("   }\n")
        f.write("}\n")


def random_guid():
    return uuid.uuid4().hex[-16:].upper()


def main(input_polygon_file, output_polygon_file, interval=0.1):
    points = parse_points(input_polygon_file)
    nsphere = welzl(points)

    sampled_points = sample_points_on_circle(nsphere.center, np.sqrt(nsphere.sqradius), interval=interval)

    write_polygon(sampled_points, output_polygon_file)


if __name__ == "__main__":
    fire.Fire(main)
