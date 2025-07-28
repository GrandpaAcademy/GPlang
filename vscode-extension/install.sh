#!/bin/bash

# GPLANG VSCode Extension Installation Script
echo "🚀 Installing GPLANG VSCode Extension"
echo "====================================="

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "❌ Node.js is not installed. Please install Node.js first."
    exit 1
fi

# Check if npm is installed
if ! command -v npm &> /dev/null; then
    echo "❌ npm is not installed. Please install npm first."
    exit 1
fi

# Check if vsce is installed
if ! command -v vsce &> /dev/null; then
    echo "📦 Installing vsce (Visual Studio Code Extension manager)..."
    npm install -g vsce
fi

# Navigate to extension directory
cd "$(dirname "$0")"

echo "📦 Installing dependencies..."
npm install

echo "🔨 Compiling TypeScript..."
npm run compile

echo "📁 Creating extension icon..."
# Copy the GPLANG logo as extension icon
mkdir -p images
cp ../assets/logo.svg images/icon.png 2>/dev/null || echo "⚠️  Logo not found, using default icon"

echo "📦 Packaging extension..."
if ! command -v vsce &> /dev/null; then
    echo "📦 Installing vsce globally..."
    npm install -g @vscode/vsce
fi
vsce package

# Get the generated .vsix file
VSIX_FILE=$(ls *.vsix | head -n 1)

if [ -f "$VSIX_FILE" ]; then
    echo "✅ Extension packaged successfully: $VSIX_FILE"
    
    # Install the extension
    echo "🔧 Installing extension to VSCode..."
    code --install-extension "$VSIX_FILE"
    
    if [ $? -eq 0 ]; then
        echo "🎉 GPLANG VSCode Extension installed successfully!"
        echo ""
        echo "📋 Features installed:"
        echo "   • Syntax highlighting for .gp files"
        echo "   • Code snippets and auto-completion"
        echo "   • GPLANG compiler integration"
        echo "   • Performance optimization hints"
        echo "   • Real-time error checking"
        echo "   • Custom GPLANG themes"
        echo ""
        echo "🔧 Commands available:"
        echo "   • Ctrl+Shift+B: Compile GPLANG file"
        echo "   • Ctrl+F5: Run GPLANG file"
        echo "   • Right-click: Optimize GPLANG file"
        echo ""
        echo "⚙️  Configure the extension in VSCode settings:"
        echo "   • File > Preferences > Settings > Extensions > GPLANG"
        echo ""
        echo "🚀 Start coding in GPLANG with full IDE support!"
    else
        echo "❌ Failed to install extension. Please install manually:"
        echo "   code --install-extension $VSIX_FILE"
    fi
else
    echo "❌ Failed to package extension"
    exit 1
fi
