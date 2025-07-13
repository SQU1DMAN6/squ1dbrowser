const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
  onDevToolsOpen: (handler) => ipcRenderer.on('devtools-open', handler),
  requestDevTools: () => ipcRenderer.send('request-devtools')
});
