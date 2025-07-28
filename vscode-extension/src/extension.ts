import * as vscode from 'vscode';
import { exec } from 'child_process';
import * as path from 'path';

export function activate(context: vscode.ExtensionContext) {
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
    const completionProvider = vscode.languages.registerCompletionItemProvider(
        'gplang',
        new GPLangCompletionProvider(),
        '.'
    );

    const hoverProvider = vscode.languages.registerHoverProvider(
        'gplang',
        new GPLangHoverProvider()
    );

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

    context.subscriptions.push(
        compileCommand,
        runCommand,
        optimizeCommand,
        completionProvider,
        hoverProvider
    );
}

function compileCurrentFile() {
    const editor = vscode.window.activeTextEditor;
    if (!editor || editor.document.languageId !== 'gplang') {
        vscode.window.showErrorMessage('No GPLANG file is currently open.');
        return;
    }

    const filePath = editor.document.fileName;
    const config = vscode.workspace.getConfiguration('gplang');
    const compilerPath = config.get<string>('compilerPath', 'gplang');
    const optimizationLevel = config.get<string>('optimizationLevel', 'aggressive');

    const command = `${compilerPath} compile --optimization=${optimizationLevel} "${filePath}"`;

    vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Compiling GPLANG file...",
        cancellable: false
    }, () => {
        return new Promise<void>((resolve, reject) => {
            exec(command, (error, stdout, stderr) => {
                if (error) {
                    vscode.window.showErrorMessage(`Compilation failed: ${error.message}`);
                    reject(error);
                } else {
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
    const compilerPath = config.get<string>('compilerPath', 'gplang');

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
    const compilerPath = config.get<string>('compilerPath', 'gplang');

    const command = `${compilerPath} optimize --level=ultra "${filePath}"`;

    vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Optimizing GPLANG file...",
        cancellable: false
    }, () => {
        return new Promise<void>((resolve, reject) => {
            exec(command, (error, stdout, stderr) => {
                if (error) {
                    vscode.window.showErrorMessage(`Optimization failed: ${error.message}`);
                    reject(error);
                } else {
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

class GPLangCompletionProvider implements vscode.CompletionItemProvider {
    provideCompletionItems(
        document: vscode.TextDocument,
        position: vscode.Position
    ): vscode.CompletionItem[] {
        const completions: vscode.CompletionItem[] = [];

        // Keywords
        const keywords = [
            'func', 'fun', 'fu', 'var', 'const', 'let', 'type', 'if', 'else', 'elif', 
            'while', 'for', 'in', 'break', 'continue', 'return', 'import', 'class', 
            'struct', 'enum', 'match', 'case', 'default', 'try', 'catch', 'finally', 
            'async', 'await', 'parallel'
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

class GPLangHoverProvider implements vscode.HoverProvider {
    provideHover(
        document: vscode.TextDocument,
        position: vscode.Position
    ): vscode.Hover | undefined {
        const range = document.getWordRangeAtPosition(position);
        const word = document.getText(range);

        const hoverInfo: { [key: string]: string } = {
            'func': 'Define a function in GPLANG (full keyword)',
            'fun': 'Define a function in GPLANG (short keyword)',
            'fu': 'Define a function in GPLANG (shortest keyword)',
            'var': 'Declare a mutable variable',
            'const': 'Declare a constant (immutable)',
            'let': 'Declare a block-scoped variable',
            'type': 'Define a custom type or type alias',
            'import': 'Import a module',
            'parallel': 'Parallel processing for ultra-fast performance',
            'print': 'Print values to the console',
            'math': 'Mathematical operations and functions',
            'string': 'String manipulation functions',
            'crypto': 'Cryptographic functions and security',
            'time': 'Date and time operations',
            'collections': 'Data structures and collections',
            'websocket': 'WebSocket communication',
            'graphql': 'GraphQL client and operations'
        };

        if (hoverInfo[word]) {
            return new vscode.Hover(new vscode.MarkdownString(`**${word}**: ${hoverInfo[word]}`));
        }

        return undefined;
    }
}

function updateDiagnostics(document: vscode.TextDocument, collection: vscode.DiagnosticCollection): void {
    const config = vscode.workspace.getConfiguration('gplang');
    const enableLinting = config.get<boolean>('enableLinting', true);

    if (!enableLinting) {
        collection.clear();
        return;
    }

    const diagnostics: vscode.Diagnostic[] = [];
    const text = document.getText();
    const lines = text.split('\n');

    lines.forEach((line, lineIndex) => {
        const trimmedLine = line.trim();

        // Skip empty lines and comments completely
        if (trimmedLine === '' || trimmedLine.startsWith('#')) {
            return;
        }

        // Check for function declarations - must start with 'func ', 'fun ', or 'fu ' and not be a comment
        const funcPattern = /^\s*(func|fun|fu)\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\([^)]*\)\s*(.*)$/;
        const funcMatch = funcPattern.exec(trimmedLine);
        
        if (funcMatch) {
            const afterParams = funcMatch[2].trim();
            // Function declaration should end with ':' 
            if (!afterParams.endsWith(':')) {
                const diagnostic = new vscode.Diagnostic(
                    new vscode.Range(lineIndex, 0, lineIndex, line.length),
                    'Function declaration should end with ":"',
                    vscode.DiagnosticSeverity.Error
                );
                diagnostics.push(diagnostic);
            }
        }

        // Check for variable declarations with proper syntax
        const varPattern = /^\s*(var|const|let|type)\s+[a-zA-Z_][a-zA-Z0-9_]*\s*=\s*.+$/;
        if (trimmedLine.match(/^\s*(var|const|let|type)\s+[a-zA-Z_][a-zA-Z0-9_]*\s*$/) && !trimmedLine.includes('=')) {
            const diagnostic = new vscode.Diagnostic(
                new vscode.Range(lineIndex, 0, lineIndex, line.length),
                'Variable declaration should include initialization',
                vscode.DiagnosticSeverity.Warning
            );
            diagnostics.push(diagnostic);
        }

        // Performance hints for for loops
        const showPerformanceHints = config.get<boolean>('showPerformanceHints', true);
        if (showPerformanceHints) {
            // Match actual for loop syntax: "for variable in iterable:"
            const forLoopPattern = /^\s*for\s+\w+\s+in\s+.+:/;
            if (forLoopPattern.test(trimmedLine) && !trimmedLine.includes('parallel')) {
                const diagnostic = new vscode.Diagnostic(
                    new vscode.Range(lineIndex, 0, lineIndex, line.length),
                    'Consider using "parallel for" for better performance',
                    vscode.DiagnosticSeverity.Information
                );
                diagnostic.tags = [vscode.DiagnosticTag.Unnecessary];
                diagnostics.push(diagnostic);
            }
        }

        // Check for missing import statements for standard library usage
        if (trimmedLine.includes('Time.now()') && !text.includes('import std.time')) {
            const diagnostic = new vscode.Diagnostic(
                new vscode.Range(lineIndex, 0, lineIndex, line.length),
                'Missing import: add "import std.time" at the top of the file',
                vscode.DiagnosticSeverity.Warning
            );
            diagnostics.push(diagnostic);
        }
    });

    collection.set(document.uri, diagnostics);
}

export function deactivate() {
    console.log('GPLANG extension deactivated');
}
