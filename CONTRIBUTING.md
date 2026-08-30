# 🤝 Contributing Guide

Благодарим ти за интереса да се присъединиш към този проект! Това ръководство ще ти помогне да направиш продуктивен вклад.

## 📋 Code of Conduct

- Бъди уважителен към всички
- Приеми конструктивната критика
- Насърчавай включване и разнообразие
- Фокусирай се върху това, което е добро за общностт

## 🐛 Докладване на грешки

Преди да докладваш грешка:

1. Провери дали вече е докладана в Issues
2. Опиши конкретния проблем
3. Включи стъпки за възпроизводство
4. Добави скринови (ако е уместно)
5. Посочи оперативната си система и браузър

### Шаблон за грешка:

```markdown
## Описание
[Ясно описание на грешката]

## Стъпки за възпроизводство
1. Направи това
2. След това това
3. Грешката се случва

## Очакивано поведение
[Какво трябва да се случи]

## Действително поведение
[Какво се случи вместо това]

## Environment
- ОС: Windows 10 / macOS / Linux
- Браузър: Chrome / Firefox / Safari
- Версия на проекта: v1.0.0
```

## ✨ Предложени на функции

За нови функции:

1. Провери Issues дали вече е предложена
2. Отвори нова Issue с детайли
3. Обсъди с мейнтейнърите преди да начнеш писането на код

### Идеи за вноски:

- 🎮 Нови игрови режими
- 🎨 UI/UX подобрения
- 📱 Мобилна оптимизация
- 🔧 Перформанс подобрения
- 📚 Документация
- 🧪 Тестов код

## 🔧 Development Setup

```bash
# Fork проекта
# Clone своят fork
git clone https://github.com/YOUR_USERNAME/labirint-game.git
cd labirint-game

# Добави upstream
git remote add upstream https://github.com/ORIGINAL_OWNER/labirint-game.git

# Инсталирай зависимости
npm install

# Обновяване от upstream
git fetch upstream
git rebase upstream/main
```

## 📝 Процес на разработка

### 1. Създай branch

```bash
git checkout -b feature/awesome-feature
# или
git checkout -b bugfix/issue-number
```

### 2. Направи промени

- Пиши чист, читаем код
- Добавяй коментари когато е необходимо
- Следвай съществуващия стил на кода
- Помни: "Код, който чупи тестовете, се отхвърля"

### 3. Commit с добро съобщение

```bash
git add .
git commit -m "feat: add awesome feature

- Детайл 1
- Детайл 2

Closes #123"
```

**Формат на commit:**
- `feat:` - Нова функция
- `fix:` - Корекция на грешка
- `docs:` - Документация
- `style:` - Форматиране, кавички, etc
- `refactor:` - Преобработка на код
- `perf:` - Перформанс подобрение
- `test:` - Добавяне на тестове

### 4. Push към своя fork

```bash
git push origin feature/awesome-feature
```

### 5. Отвори Pull Request

- Използвай PR шаблон (ако е наличен)
- Обясни какво правиш и защо
- Свързани Issues: `Closes #123`
- Добавяй скринови за UI промени
- Жди за review

### 6. Code Review

- Отговори на коментари
- Направи препоръчани промени
- Push нови commits (не force push!)
- Жди за одобрение

### 7. Merge

Когато PR е одобрен, мейнтейнърите ще го мерджат.

## 📚 Кодов стил

### C++
```cpp
// Используй camelCase за функции и променливи
void myFunction() {
    // Коментари на английски
    int myVariable = 0;
}

// Класове с PascalCase
class MyClass {
    private:
        int value;
    public:
        void setValue(int v) { value = v; }
};
```

### JavaScript
```javascript
// Используй const/let, избегни var
const MAX_HEALTH = 10.0;
let currentHealth = 1.0;

// Функции с camelCase
function calculateDamage(enemy, player) {
    // Коментари ако е необходимо
    return Math.abs(enemy.x - player.x);
}

// Strings с backticks
const message = `Играч ${name} се присъедини`;
```

### Форматиране
- Отступи: 4 пространства (C++) или 2 пространства (JS)
- Max линия: 100 символа
- Никога не пращай форматирани-само промени

## 🧪 Тестване

Преди PR:

```bash
# Провери дали се компилира
npm run build:wasm

# Локално тестиране
npm start

# Ръчно тестирай функцията
```

## 📖 Документация

За всяка нова публична функция:

```cpp
/// Calculates the Manhattan distance between two points
/// @param x1 First X coordinate
/// @param y1 First Y coordinate
/// @param x2 Second X coordinate  
/// @param y2 Second Y coordinate
/// @return Distance as integer
int calculateDistance(int x1, int y1, int x2, int y2);
```

## 🚀 Release Process

Мейнтейнърите създават релизи. За версионирането следваме [Semantic Versioning](https://semver.org/):

- **MAJOR** - Неcompatible промени
- **MINOR** - Нови функции (backward compatible)
- **PATCH** - Bug фиксове

## 💬 Community

- 💬 Дискусии в Issues
- 📧 Email: your-email@example.com
- 🐦 Twitter: @your_twitter

## ⚖️ License

Чрез приносяне, согласяваш се че твоята работа ще бъде лицензирана под MIT License.

---

**Благодарим за вашия вклад! 🌟**
