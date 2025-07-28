"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
const vscode = require("vscode");
const child_process_1 = require("child_process");
function activate(context) {
    console.log('GPLANG extension is now active!');
    // Register commands
    const compileCommand = vscode.commands.registerCommand('gplang.compile', () => {
        compileCurrentFile();
    });
    const runCommand = vscode.commands.registerCommand('gplang.run', () => {
        runCurrentFile();
    });
    const optimizeCommand = vscode.commands.registerCommand('gplang.optimize', () => {
        optimizeCurrentFile();
    });
    // Register providers
    const completionProvider = vscode.languages.registerCompletionItemProvider('gplang', new GPLangCompletionProvider(), '.');
    const hoverProvider = vscode.languages.registerHoverProvider('gplang', new GPLangHoverProvider());
    const diagnosticCollection = vscode.languages.createDiagnosticCollection('gplang');
    context.subscriptions.push(diagnosticCollection);
    // Watch for file changes to provide diagnostics
    const activeEditor = vscode.window.activeTextEditor;
    if (activeEditor && activeEditor.document.languageId === 'gplang') {
        updateDiagnostics(activeEditor.document, diagnosticCollection);
    }
    vscode.window.onDidChangeActiveTextEditor(editor => {
        if (editor && editor.document.languageId === 'gplang') {
            updateDiagnostics(editor.document, diagnosticCollection);
        }
    });
    vscode.workspace.onDidChangeTextDocument(event => {
        if (event.document.languageId === 'gplang') {
            updateDiagnostics(event.document, diagnosticCollection);
        }
    });
    context.subscriptions.push(compileCommand, runCommand, optimizeCommand, completionProvider, hoverProvider);
}
exports.activate = activate;
function compileCurrentFile() {
    const editor = vscode.window.activeTextEditor;
    if (!editor || editor.document.languageId !== 'gplang') {
        vscode.window.showErrorMessage('No GPLANG file is currently open.');
        return;
    }
    const filePath = editor.document.fileName;
    const config = vscode.workspace.getConfiguration('gplang');
    const compilerPath = config.get('compilerPath', 'gplang');
    const optimizationLevel = config.get('optimizationLevel', 'aggressive');
    const command = `${compilerPath} compile --optimization=${optimizationLevel} "${filePath}"`;
    vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Compiling GPLANG file...",
        cancellable: false
    }, () => {
        return new Promise((resolve, reject) => {
            (0, child_process_1.exec)(command, (error, stdout, stderr) => {
                if (error) {
                    vscode.window.showErrorMessage(`Compilation failed: ${error.message}`);
                    reject(error);
                }
                else {
                    vscode.window.showInformationMessage('Compilation successful!');
                    if (stdout) {
                        console.log(stdout);
                    }
                    resolve();
                }
            });
        });
    });
}
function runCurrentFile() {
    const editor = vscode.window.activeTextEditor;
    if (!editor || editor.document.languageId !== 'gplang') {
        vscode.window.showErrorMessage('No GPLANG file is currently open.');
        return;
    }
    const filePath = editor.document.fileName;
    const config = vscode.workspace.getConfiguration('gplang');
    const compilerPath = config.get('compilerPath', 'gplang');
    const command = `${compilerPath} run "${filePath}"`;
    const terminal = vscode.window.createTerminal('GPLANG');
    terminal.sendText(command);
    terminal.show();
}
function optimizeCurrentFile() {
    const editor = vscode.window.activeTextEditor;
    if (!editor || editor.document.languageId !== 'gplang') {
        vscode.window.showErrorMessage('No GPLANG file is currently open.');
        return;
    }
    const filePath = editor.document.fileName;
    const config = vscode.workspace.getConfiguration('gplang');
    const compilerPath = config.get('compilerPath', 'gplang');
    const command = `${compilerPath} optimize --level=ultra "${filePath}"`;
    vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Optimizing GPLANG file...",
        cancellable: false
    }, () => {
        return new Promise((resolve, reject) => {
            (0, child_process_1.exec)(command, (error, stdout, stderr) => {
                if (error) {
                    vscode.window.showErrorMessage(`Optimization failed: ${error.message}`);
                    reject(error);
                }
                else {
                    vscode.window.showInformationMessage('Optimization complete!');
                    if (stdout) {
                        const outputChannel = vscode.window.createOutputChannel('GPLANG Optimizer');
                        outputChannel.appendLine(stdout);
                        outputChannel.show();
                    }
                    resolve();
                }
            });
        });
    });
}
class GPLangCompletionProvider {
    provideCompletionItems(document, position) {
        const completions = [];
        // Keywords
        const keywords = [
            'func', 'var', 'const', 'if', 'else', 'elif', 'while', 'for', 'in',
            'break', 'continue', 'return', 'import', 'class', 'struct', 'enum',
            'match', 'case', 'default', 'try', 'catch', 'finally', 'async', 'await'
        ];
        keywords.forEach(keyword => {
            const item = new vscode.CompletionItem(keyword, vscode.CompletionItemKind.Keyword);
            item.detail = `GPLANG keyword`;
            completions.push(item);
        });
        // Built-in functions
        const builtins = [
            { name: 'print', detail: 'Print to console', snippet: 'print(${1:value})' },
            { name: 'len', detail: 'Get length', snippet: 'len(${1:object})' },
            { name: 'str', detail: 'Convert to string', snippet: 'str(${1:value})' },
            { name: 'int', detail: 'Convert to integer', snippet: 'int(${1:value})' },
            { name: 'float', detail: 'Convert to float', snippet: 'float(${1:value})' },
            { name: 'range', detail: 'Create range', snippet: 'range(${1:start}, ${2:end})' }
        ];
        builtins.forEach(builtin => {
            const item = new vscode.CompletionItem(builtin.name, vscode.CompletionItemKind.Function);
            item.detail = builtin.detail;
            item.insertText = new vscode.SnippetString(builtin.snippet);
            completions.push(item);
        });
        // Module completions
        const modules = [
            'math', 'string', 'crypto', 'time', 'collections', 'fs', 'os', 'net', 'json',
            'websocket', 'graphql', 'socketio'
        ];
        modules.forEach(module => {
            const item = new vscode.CompletionItem(module, vscode.CompletionItemKind.Module);
            item.detail = `GPLANG ${module} module`;
            completions.push(item);
        });
        return completions;
    }
}
class GPLangHoverProvider {
    provideHover(document, position) {
        const range = document.getWordRangeAtPosition(position);
        const word = document.getText(range);
        const hoverInfo = {
            'func': 'Define a function in GPLANG',
            'var': 'Declare a variable',
            'const': 'Declare a constant',
            'import': 'Import a module',
            'print': 'Print values to the console',
            'math': 'Mathematical operations and functions',
            'string': 'String manipulation functions',
            'crypto': 'Cryptographic functions and security',
            'time': 'Date and time operations',
            'collections': 'Data structures and collections',
            'websocket': 'WebSocket communication',
            'graphql': 'GraphQL client and operations',
            'parallel': 'Parallel processing for ultra-fast performance'
        };
        if (hoverInfo[word]) {
            return new vscode.Hover(new vscode.MarkdownString(`**${word}**: ${hoverInfo[word]}`));
        }
        return undefined;
    }
}
function updateDiagnostics(document, collection) {
    const config = vscode.workspace.getConfiguration('gplang');
    const enableLinting = config.get('enableLinting', true);
    if (!enableLinting) {
        collection.clear();
        return;
    }
    const diagnostics = [];
    const text = document.getText();
    const lines = text.split('\n');
    lines.forEach((line, lineIndex) => {
        // Simple syntax checking
        if (line.includes('func') && !line.includes(':')) {
            const diagnostic = new vscode.Diagnostic(new vscode.Range(lineIndex, 0, lineIndex, line.length), 'Function declaration should end with ":"', vscode.DiagnosticSeverity.Error);
            diagnostics.push(diagnostic);
        }
        // Performance hints
        const showPerformanceHints = config.get('showPerformanceHints', true);
        if (showPerformanceHints) {
            if (line.includes('for') && !line.includes('parallel')) {
                const diagnostic = new vscode.Diagnostic(new vscode.Range(lineIndex, 0, lineIndex, line.length), 'Consider using "parallel for" for better performance', vscode.DiagnosticSeverity.Information);
                diagnostic.tags = [vscode.DiagnosticTag.Unnecessary];
                diagnostics.push(diagnostic);
            }
        }
    });
    collection.set(document.uri, diagnostics);
}
function deactivate() {
    console.log('GPLANG extension deactivated');
}
exports.deactivate = deactivate;
//# sourceMappingURL=extension.js.map