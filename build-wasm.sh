#!/bin/bash
# Build script for local WebAssembly compilation
# Usage: ./build-wasm.sh

echo "🔨 Building WebAssembly..."

# Check if Emscripten is installed
if ! command -v emcc &> /dev/null
then
    echo "❌ Emscripten not found. Install from:"
    echo "   https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi

echo "✓ Emscripten found"

# Create build directory
mkdir -p build

# Run CMake with Emscripten
echo "🔧 Running CMake..."
emcmake cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
echo "⚙️ Compiling..."
cmake --build build --config Release

# Copy to public directory
echo "📦 Copying files to public/"
cp build/game.js public/ 2>/dev/null || echo "⚠️ game.js not found"
cp build/game.wasm public/ 2>/dev/null || echo "⚠️ game.wasm not found"
cp build/game.worker.js public/ 2>/dev/null || echo "⚠️ game.worker.js not found (optional)"

echo "✅ Build complete!"
echo "📁 Check public/ directory for output files"
