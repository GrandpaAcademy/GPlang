# GPLANG VSCode Extension Troubleshooting

## Syntax Highlighting Issues

### Problem: No syntax highlighting for .gp files

**Solution 1: Check Language Mode**
1. Open your `.gp` file in VSCode
2. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
3. Type "Change Language Mode"
4. Select "GPLANG" from the list

**Solution 2: Force Language Association**
1. Open a `.gp` file
2. Click on the language indicator in the bottom-right corner
3. Select "Configure File Association for '.gp'"
4. Choose "GPLANG"

**Solution 3: Reload VSCode**
1. Press `Ctrl+Shift+P`
2. Type "Developer: Reload Window"
3. Press Enter

### Problem: Syntax highlighting is incorrect or incomplete

**Solution 1: Update Extension**
```bash
cd vscode-extension
./install.sh
```

**Solution 2: Manual Reinstall**
```bash
code --uninstall-extension gplang-team.gplang
code --install-extension gplang-1.0.1.vsix --force
```

**Solution 3: Clear VSCode Cache**
1. Close VSCode completely
2. Delete VSCode workspace cache:
   - Windows: `%APPDATA%\Code\User\workspaceStorage`
   - macOS: `~/Library/Application Support/Code/User/workspaceStorage`
   - Linux: `~/.config/Code/User/workspaceStorage`
3. Restart VSCode

### Problem: Colors don't look right

**Solution: Use GPLANG Theme**
1. Press `Ctrl+K Ctrl+T`
2. Select "GPLANG Dark" or "GPLANG Light"

## Extension Features Not Working

### Problem: Compile/Run commands not available

**Check 1: Extension is Active**
1. Press `Ctrl+Shift+X` to open Extensions
2. Search for "GPLANG"
3. Ensure it's enabled

**Check 2: File Type Recognition**
1. Open a `.gp` file
2. Check bottom-right corner shows "GPLANG"
3. If not, follow language mode steps above

**Check 3: Command Palette**
1. Press `Ctrl+Shift+P`
2. Type "GPLANG"
3. You should see compile/run commands

### Problem: Auto-completion not working

**Solution 1: Trigger IntelliSense**
1. Type a few characters
2. Press `Ctrl+Space` to trigger suggestions

**Solution 2: Check Settings**
1. Go to File > Preferences > Settings
2. Search for "GPLANG"
3. Ensure "Enable Linting" is checked

## Performance Issues

### Problem: Extension is slow

**Solution 1: Disable Performance Hints**
```json
{
  "gplang.showPerformanceHints": false
}
```

**Solution 2: Reduce Linting**
```json
{
  "gplang.enableLinting": false
}
```

## Installation Issues

### Problem: Extension won't install

**Solution 1: Check VSCode Version**
- Requires VSCode 1.74.0 or higher
- Update VSCode if needed

**Solution 2: Manual Installation**
```bash
cd /path/to/gplang/vscode-extension
npm install
npm run compile
vsce package --no-yarn
code --install-extension gplang-1.0.1.vsix
```

**Solution 3: Check Permissions**
- Ensure you have write permissions to VSCode extensions folder
- Run VSCode as administrator if needed (Windows)

### Problem: Dependencies missing

**Install Node.js and npm:**
- Download from https://nodejs.org/
- Verify: `node --version` and `npm --version`

**Install vsce:**
```bash
npm install -g vsce
```

## Common Error Messages

### "Extension host terminated unexpectedly"

**Solution:**
1. Press `Ctrl+Shift+P`
2. Type "Developer: Restart Extension Host"
3. If problem persists, reload window

### "Cannot find module"

**Solution:**
```bash
cd vscode-extension
npm install
npm run compile
```

### "Language configuration not found"

**Solution:**
1. Check `language-configuration.json` exists
2. Reinstall extension
3. Restart VSCode

## Testing Syntax Highlighting

Run the test script:
```bash
cd vscode-extension
./test-syntax.sh
```

This creates a comprehensive test file that demonstrates all syntax highlighting features.

## Expected Syntax Colors (GPLANG Dark Theme)

- **Keywords** (`func`, `if`, `for`): Purple/Pink (#C586C0)
- **Strings**: Orange (#CE9178)
- **Numbers**: Light Green (#B5CEA8)
- **Comments**: Green Italic (#6A9955)
- **Function Names**: Yellow (#DCDCAA)
- **Built-in Functions**: Teal (#4EC9B0)
- **Variables**: Light Blue (#9CDCFE)
- **Operators**: White (#D4D4D4)
- **Module Names**: Teal Bold (#4EC9B0)

## Getting Help

1. **Check Extension Output:**
   - View > Output > Select "GPLANG" from dropdown

2. **Enable Developer Tools:**
   - Help > Toggle Developer Tools
   - Check Console for errors

3. **Report Issues:**
   - GitHub: https://github.com/gplang/gplang/issues
   - Include VSCode version, extension version, and error messages

## Quick Fixes

**Reset Everything:**
```bash
# Uninstall extension
code --uninstall-extension gplang-team.gplang

# Reinstall
cd vscode-extension
./install.sh

# Restart VSCode
```

**Force Language Mode:**
1. Open `.gp` file
2. Press `Ctrl+Shift+P`
3. Type "Change Language Mode"
4. Select "GPLANG"

**Test Installation:**
```bash
cd vscode-extension
./test-syntax.sh
```
