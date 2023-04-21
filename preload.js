const { contextBridge, ipcRenderer } = require('electron')

contextBridge.exposeInMainWorld('electronAPI',{
  CellEvent: (CellId) => ipcRenderer.send('CellEvent', CellId),
  StartButton: () =>  ipcRenderer.invoke('dialog:StartButton'),
  UpdateBoard: (callback) => ipcRenderer.on('update-board', callback)
})

