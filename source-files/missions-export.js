/**
 * Импортирует данные о сценариях из Reforger Workshop по одному или нескольким URL.
 *
 * @param {string|string[]} urls Один URL или массив URL'ов
 * @return {Array[]} Массив данных для вставки в таблицу
 * @customfunction
 */
function IMPORTWORKSHOPDATA(urls) {
  if (!urls) {
    throw new Error("Передайте один или несколько URL.");
  }

  // Приводим к массиву
  if (!Array.isArray(urls)) {
    urls = [[urls]]; // Превращаем одиночный элемент в двумерный массив, если пользователь передал один URL
  }

  // Плоский массив всех URL
  const flatUrls = urls.flat().filter(Boolean);

  const output = [];
  
  // Заголовки
  output.push(["Название", "Описание", "Режим игры", "Игроков", "ScenarioID", "Author", "Addon Name"]);

  for (const url of flatUrls) {
    const asset = fetchWorkshopAsset(url);

    for (const scenario of asset.scenarios) {
      output.push([
        scenario.name,
        scenario.description,
        scenario.gameMode,
        scenario.playerCount,
        scenario.gameId,
        scenario.authorName,
        asset.name
      ]);
    }
  }

  return output;
}

// Вспомогательная функция: загружает данные одного аддона
function fetchWorkshopAsset(url) {
  const response = UrlFetchApp.fetch(url);
  const html = response.getContentText();

  const jsonStart = html.indexOf('<script id="__NEXT_DATA__" type="application/json">');
  const jsonEnd = html.indexOf('</script>', jsonStart);
  if (jsonStart === -1 || jsonEnd === -1) {
    throw new Error("Не удалось найти JSON данные в HTML.");
  }

  const jsonText = html.substring(
    html.indexOf('>', jsonStart) + 1,
    jsonEnd
  );

  const data = JSON.parse(jsonText);
  return data.props.pageProps.asset;
}
