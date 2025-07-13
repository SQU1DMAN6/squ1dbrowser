const { app, BrowserWindow, Menu, ipcMain } = require('electron')

const createWindow = () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    minWidth: 800,
    minHeight: 600,
    webPreferences: {
      devTools: true,
      nodeIntegration: true,
      javascript: true,
      webSecurity: true,
      images: true,
      webgl: true,
      scrollBounce: true,
      contextIsolation: false,
      webviewTag: true
    }
  })

  win.loadFile('index.html')

  const template = [
    {
      label: 'Browser',
      submenu: [
        {
          label: "Devtools",
          accelerator: 'F12',
          click: () => {
            win.webContents.send('devtools-open')
          }
        },
      ]
    },
    {
      label: 'File',
      submenu: [
        {
          label: 'Quit App',
          accelerator: 'CommandOrControl+Q',
          click: () => {
            app.quit()
          }
        }
      ]
    }
  ]

  const menu = Menu.buildFromTemplate(template);
  Menu.setApplicationMenu(menu)
}

app.whenReady().then(() => {
  try{
    createWindow()
  }catch(err){
    console.log(err)
  }
  
})