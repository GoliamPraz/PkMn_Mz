# Лабиринт - WebAssembly Maze Game 🎮

Интерактивна игра с лабиринт, написана на C++ с raylib и компилирана в WebAssembly. Поддържа единочна и мултиплеър режим.

## 🌐 Play Online

Играй в браузър: https://labirint-game.onrender.com

## ✨ Features

- 🕹️ **Единочна игра** - Класическия лабиринт режим
- 👥 **Мултиплеър** - Играй с приятели онлайн
- 🎨 **Интерактивен UI** - Съвременен дизайн
- 💻 **WebAssembly** - Бързо и ефективно
- 🔄 **Real-time** - WebSocket синхронизиране
- 📱 **Отзивчив дизайн** - Работи на всеки екран

## 🎮 Управление

- **↑↓←→** или **WASD** - Движение
- **Зелени точки** - Събери ги за бонус
- **Врагове** - 
  - Черни: Преследват те
  - Сини (по време на бонус): Бягат от теб
- **Живот** - Начало 1.00, макс 10.00
- **Точки** - Събирай точки като поразиш враговете

## 🛠️ Installation & Development

### Изисквания

- Node.js (>= 16)
- Emscripten (за WebAssembly компилиране)
- CMake (>= 3.16)

### Build

```bash
# Инсталирай зависимости
npm install

# Компилирай WebAssembly версия
npm run build:wasm

# Копирай файлове в public папка
npm run build:web
```

### Run Locally

```bash
# Стартирай сървър
npm start

# Отвори браузър
open http://localhost:3000
```

## 📦 Deployment

### На Render.com

1. Пуш код към GitHub
2. Свържи GitHub репозитория с Render.com
3. Render автоматично ще компилира и хоства

```bash
# Ръчен деплой
git push origin main
```

### На Heroku

```bash
heroku create labirint-game
git push heroku main
```

## 🏗️ Project Structure

```
.
├── main.cpp              # C++ игровата логика
├── server.js             # Node.js WebSocket сървър
├── client.js             # JavaScript клиент
├── shell.html            # HTML Emscripten shell
├── CMakeLists.txt        # Emscripten build конфиг
├── package.json          # Node.js зависимости
├── Procfile              # Heroku деплой
├── render.yaml           # Render.com конфиг
└── public/               # Генериран WebAssembly файлове
    ├── game.js
    ├── game.wasm
    └── index.html
```

## 🎯 Game Rules

### Единочна игра
- Събирай зелени точки за бонус време
- Избягай черните враги
- По време на бонус, сините враги бягат от теб
- Максимизирай своя живот до 10.00

### Мултиплеър
- Същите правила, но със синхронизирани враги
- Вижда се списък на онлайн играчите
- Реално време обновяване на позициите

## 🔒 API

### WebSocket Events

```javascript
// Присъединяване към игра
{ type: 'join', playerId: 'xxx', playerName: 'Име' }

// Обновяване на игровото състояние
{ type: 'game_state', playerId: 'xxx', state: {...} }

// Получаване на списък на играчи
{ type: 'players_update', players: [...] }

// Ping/Pong за heartbeat
{ type: 'ping' } → { type: 'pong' }
```

### REST API

```
GET /api/players        # Списък на активни играчи
GET /api/stats         # Статистика на сървъра
```

## 🐛 Known Issues

- WebAssembly компилирането изисква Emscripten инстанция
- Мултиплеър синхронизирането има минимална закъснение

## 📝 License

MIT License - вижте LICENSE файла

## 👨‍💻 Contributing

1. Fork репозитория
2. Създай feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit промени (`git commit -m 'Add some AmazingFeature'`)
4. Push към branch (`git push origin feature/AmazingFeature`)
5. Отвори Pull Request

## 🎓 Technologies

- **C++** - Игрова логика
- **raylib** - Graphics & windowing
- **Emscripten** - C++ to WebAssembly
- **WebAssembly (WASM)** - Browser runtime
- **Node.js** - Backend сървър
- **WebSocket** - Real-time communication
- **Express.js** - Web framework

## 📧 Contact

За въпроси и предложения, свържи се или отвори Issue.

---

**Забавлявай се с Лабиринта! 🚀**
