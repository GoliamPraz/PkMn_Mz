# 🚀 Брз старт гайд

## За разработчици

### 1. Подготовка на окръжението

```bash
# Инсталирай Emscripten (един път)
# За Linux/Mac:
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# За Windows:
# Изтегли инсталатора от https://emscripten.org/docs/getting_started/downloads.html
```

### 2. Клониране на проекта

```bash
git clone https://github.com/YOUR_USERNAME/labirint-game.git
cd labirint-game
npm install
```

### 3. Локално разработване

```bash
# Компилирай WebAssembly
npm run build:wasm

# Копирай във public папка
npm run build:web

# Стартирай сървър
npm start

# Отвори http://localhost:3000
```

## За геймъри

### Играй онлайн

1. Отвори https://labirint-game.onrender.com
2. Въведи своето име
3. Избери режим (Единочна или Мултиплеър)
4. Натисни "Начало"

### Управление

- **Стрелки** или **WASD** - Движение
- **Бягай** от враговете
- **Събери** зелени точки за бонус
- **Максимизирай** живота си до 10.00

## Деплой на Render.com

### Стъпка 1: GitHub Repository

```bash
# Инициализирай Git репозиторий
git init
git add .
git commit -m "Initial commit"
git branch -M main

# Пуш към GitHub
git remote add origin https://github.com/YOUR_USERNAME/labirint-game.git
git push -u origin main
```

### Стъпка 2: Свързване с Render.com

1. Отвори https://render.com
2. Регистрирай се/Влез
3. Натисни "New +" → "Web Service"
4. Свържи GitHub акаунта
5. Избери `labirint-game` репозиторий
6. Попълни:
   - **Name**: labirint-game
   - **Environment**: Node
   - **Build Command**: `npm install`
   - **Start Command**: `npm start`
7. Натисни "Deploy"

### Стъпка 3: Автоматичен деплой

Всеки път когато натиснеш към `main`:
- GitHub Actions ще изпълни тестовете
- Render автоматично ще деплойра новата версия

## Структура на проекта

```
labirint-game/
├── main.cpp              # C++ игрова логика
├── server.js             # Node.js сървър
├── client.js             # JavaScript клиент
├── shell.html            # Emscripten HTML shell
├── CMakeLists.txt        # CMake конфиг за WASM
├── package.json          # Node.js зависимости
├── Procfile              # Heroku деплой
├── render.yaml           # Render.com конфиг
├── .github/workflows/    # CI/CD
├── public/               # Генерирани файлове
│   ├── index.html
│   ├── game.js
│   └── game.wasm
└── README.md
```

## Проблеми и решения

### "Emscripten не е намерен"

```bash
# Провери инсталацията
emcc --version

# Ако е инсталиран, активирай го:
source ./emsdk_env.sh  # Linux/Mac
```

### "Port 3000 е зает"

```bash
# Променя порта
PORT=3001 npm start
```

### "WebSocket не се свързва"

- Провери че сървърът работи
- Провери дали браузърът поддържа WebSocket
- Вижте браузърната конзола за грешки

## Типични задачи

### Добавяне на нов feature

1. Създай branch: `git checkout -b feature/my-feature`
2. Направи промени
3. Тестирай локално
4. Commit: `git commit -am "Add my feature"`
5. Push: `git push origin feature/my-feature`
6. Отвори Pull Request

### Обновяване на зависимости

```bash
npm update
git add package.json package-lock.json
git commit -m "Update dependencies"
git push origin main
```

### Просмотр на логове

```bash
# Локално
npm start  # Вижда всички логове в конзолата

# На Render.com
# Отвори Dashboard → labirint-game → Logs
```

## Линкове

- 📖 [Raylib документация](https://www.raylib.com/)
- 🔧 [Emscripten гайд](https://emscripten.org/)
- 📝 [Node.js документация](https://nodejs.org/)
- 🚀 [Render.com документация](https://render.com/docs)

---

Щастлива разработка! 🎮
