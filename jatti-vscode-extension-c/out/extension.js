"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;
const vscode = require("vscode");
const path = require("path");
const fs = require("fs");
const child_process_1 = require("child_process");
const KEYWORDS = [
    'sun_we', 'ja_we', 'chal_oye', 'ban', 'chilla_we', 'fuddu_chiz',
    'je', 'nahin_taan_je', 'nahin_taan', 'har_ek', 'jadon_tak',
    'roko_oye_roko', 'chalo_oye_chalo', 'kaam', 'wapas_kar',
    'chal_koshish_karle', 'pakad', 'vadha_hai', 'nikka_hai',
    'barabar', 'barabar_nahi_hai', 'vadha_ya_barabar', 'nikka_ya_barabar',
    'sach', 'jhoot', 'khaali', 'ate', 'ya_te', 'nahi', 'te'
];
const BUILTINS = [
    'range_banao', 'kinna_lamba', 'kism', 'likh', 'padh',
    'ganao', 'sab_ton_vaddha', 'sab_ton_chhota', 'sorted', 'reversed',
    'vada_likha', 'chhota_likha', 'saf_karo', 'vand_karo', 'badal_de', 'shuru_hunda', 'khatam_hunda', 'dhundh_ja'
];
let jattiOutputChannel;
function activate(context) {
    jattiOutputChannel = vscode.window.createOutputChannel('Jatti');
    const runDisposable = vscode.commands.registerCommand('jatti.runFile', async () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showErrorMessage('Open a .jatti file first.');
            return;
        }
        if (editor.document.languageId !== 'jatti') {
            vscode.window.showErrorMessage('Active file is not a Jatti file.');
            return;
        }
        if (editor.document.isUntitled) {
            const saved = await editor.document.save();
            if (!saved) {
                vscode.window.showErrorMessage('Save the file before running.');
                return;
            }
        }
        else if (editor.document.isDirty) {
            await editor.document.save();
        }
        const filePath = editor.document.fileName;
        const runtimePath = resolveRuntimePath(context);
        if (!runtimePath) {
            vscode.window.showErrorMessage('Jatti runtime not found. Put jatti.exe at extension runtime/win32/jatti.exe or set jatti.runtimePath.');
            return;
        }
        jattiOutputChannel.clear();
        jattiOutputChannel.show(true);
        jattiOutputChannel.appendLine(`Running: ${filePath}\n`);
        try {
            const quotedRuntime = `"${runtimePath}"`;
            const quotedFile = `"${filePath}"`;
            const commandLine = `${quotedRuntime} run ${quotedFile}`;
            const output = (0, child_process_1.execSync)(commandLine, {
                encoding: 'utf8',
                stdio: ['pipe', 'pipe', 'pipe']
            });
            if (output) {
                jattiOutputChannel.append(output);
            }
            jattiOutputChannel.appendLine('\n✅ Execution complete');
        }
        catch (error) {
            if (error.stdout) {
                jattiOutputChannel.append(error.stdout);
            }
            if (error.stderr) {
                jattiOutputChannel.append(error.stderr);
            }
            jattiOutputChannel.appendLine('\n❌ Execution failed');
        }
    });
    const completionDisposable = vscode.languages.registerCompletionItemProvider({ language: 'jatti' }, {
        provideCompletionItems() {
            const keywordItems = KEYWORDS.map((word) => {
                const item = new vscode.CompletionItem(word, vscode.CompletionItemKind.Keyword);
                item.insertText = word;
                return item;
            });
            const builtinItems = BUILTINS.map((name) => {
                const item = new vscode.CompletionItem(name, vscode.CompletionItemKind.Function);
                item.insertText = `${name}`;
                return item;
            });
            return [...keywordItems, ...builtinItems];
        }
    });
    context.subscriptions.push(runDisposable, completionDisposable);
}
function deactivate() {
    if (jattiOutputChannel) {
        jattiOutputChannel.dispose();
    }
}
function resolveRuntimePath(context) {
    const configured = vscode.workspace.getConfiguration('jatti').get('runtimePath')?.trim();
    if (configured && fs.existsSync(configured)) {
        return configured;
    }
    if (process.platform === 'win32') {
        const bundled = path.join(context.extensionPath, 'runtime', 'win32', 'jatti.exe');
        if (fs.existsSync(bundled)) {
            return bundled;
        }
    }
    return undefined;
}
//# sourceMappingURL=extension.js.map