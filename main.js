const { app, BrowserWindow, ipcMain, dialog, Menu } = require('electron')
const child_process = require('node:child_process');
const { create } = require('node:domain');
const fs = require('node:fs');
function createEmptyArray() {
  arr = new Array(64);

  for (i = 0; i < 64; i++) {
    arr[i] = 'None';
  }


  return arr;
}

function createStartPossion() {
  arr = new Array(64);

  for (i = 0; i < 64; i++) {
    arr[i] = 'None';
  }

  arr[0] = 'Brook';
  arr[1] = 'Bknight';
  arr[2] = 'Bbishop';
  arr[3] = 'Bking';
  arr[4] = 'Bqueen';
  arr[5] = 'Bbishop';
  arr[6] = 'Bknight';
  arr[7] = 'Brook';

  for (i = 0; i < 8; i++) {
    arr[8 + i] = 'Bpawn';
  }

  for (i = 0; i < 8; i++) {
    arr[8 + i] = 'Bpawn';
  }

  arr[56 + 0] = 'Wrook';
  arr[56 + 1] = 'Wknight';
  arr[56 + 2] = 'Wbishop';
  arr[56 + 3] = 'Wking';
  arr[56 + 4] = 'Wqueen';
  arr[56 + 5] = 'Wbishop';
  arr[56 + 6] = 'Wknight';
  arr[56 + 7] = 'Wrook';

  for (i = 0; i < 8; i++) {
    arr[48 + i] = 'Wpawn';
  }

  return arr;
}

function createOtherPossion() {
  arr = new Array(64);

  for (i = 0; i < 64; i++) {
    arr[i] = 'None';
  }
  for (i = 0; i < 8; i++) {
    arr[i] = 'Wking';
  }

  return arr;
}

function createOtherPossion2() {
  arr = new Array(64);

  for (i = 0; i < 64; i++) {
    arr[i] = 'None';
  }
  for (i = 0; i < 8; i++) {
    arr[i] = 'Wqueen';
  }

  return arr;
}

//Parse data
function makeNecessaryMassive(str) {

  if (str[0] == 'P') {
    return null;
  } else {
    arr = new Array(64);
    //console.log(str.substr(13))//.split(' ').map(Number));

    strWithotLetter = str.substr(13)

    a = 0;
    sign = 1;
    k = 0;
    flag = false;

    for (i = 0; i < strWithotLetter.length; i++) {

      c = strWithotLetter[i];
      if (flag && (c == '\n' || c == ' ')) {

        a = a * sign;
        arr[k] = a;
        k++;
        sign = 1;
        flag = false;
      }
      if (c == '-') {
        sign = -1;
      }

      if (c >= '0' && c <= '9') {
        a = Number(strWithotLetter[i]);
        flag = true;
      }
    }

    //console.log(arr);

    NecessaryArray = new Array(64);

    for (i = 0; i < 64; i++) {
      if (arr[i] > 0) {
        NecessaryArray[i] = 'W';
      }
      if (arr[i] < 0) {
        NecessaryArray[i] = 'B';
      }

      switch (Math.abs(arr[i])) {
        case 0:
          NecessaryArray[i] = 'None';
          break
        case 1:
          NecessaryArray[i] += 'queen';
          break
        case 2:
          NecessaryArray[i] += 'king';
          break
        case 3:
          NecessaryArray[i] += 'bishop';
          break
        case 4:
          NecessaryArray[i] += 'knight';
          break
        case 5:
          NecessaryArray[i] += 'rook';
          break
        case 6:
          NecessaryArray[i] += 'pawn';
          break

        default:
          NecessaryArray[i] = 'None';
      }
    }
    arr2 = new Array(64);

    for (i = 0; i < 8; i++) {
      for (j = 0; j < 8; j++) {
        arr2[56 - i * 8 + j] = NecessaryArray[8 * i + j];
      }
    }

    for (i = 0; i < 64; i++) {
      NecessaryArray[i] = arr2[i];
    }

    //console.log(NecessaryArray);
    return NecessaryArray;
  }
}
//Parse data for Sel Cells Array
function makeSelCellsArray(str) {
  if (str[0] == 'P') {
    arr = new Array;

    //console.log(str.substr(14));

    CertainStr = str.substr(14);

    if (CertainStr[0] == '0') {
      return null;
    }
    else {

      flagNum = true;
      flagStart = false;
      flag = false;
      s = '';
      k = 0;

      for (i = 0; i < CertainStr.length; i++) {


        c = CertainStr[i];
        if (!(c >= '0' && c <= '9')){
          flagNum = false;
        }
        if (!flagNum && (c != ' ' &&  c != '\n')){
          flagStart = true;
        }
        if (flagStart){
          if (flag && (c == '\n' || c == ' ')){
            arr[k] = s;
            k++;
            s = '';
            flag = false
          }

          if ((c >= '0' && c <= '9')){
            flag = true;
            s+=c;
          }

          if ((c >= 'a' && c <= 'z')){
            s+=c;
          }
        }

      }

      return arr;

    }
  } else {
    return null;
  }
}

const path = require('path');

//Создание рендера
function createWindow() {
  const mainWindow = new BrowserWindow({
    icon: 'images/Bking.png',
    contextIsolation: false,
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })

  mainWindow.loadFile('index.html')

  return mainWindow;
}


app.whenReady().then(() => {

  const child_process = require('node:child_process');

  const subProcessOnCPP = child_process.spawn('CPP.exe');

  //Organization of communication between processes///////////////
  //Array of figures
  var ChessMassive = createEmptyArray()

  flagGameBegin = false;

  //On-event. Hadnler Start Button
  ipcMain.handle('dialog:StartButton', () => {

    arr = createStartPossion();

    for (i = 0; i < 64; i++) {
      ChessMassive[i] = arr[i];
    }

    flagGameBegin = true;
    return ChessMassive;
  });

  //On-event. Handler Cell Event
  //Write Cell id to CPP process
  ipcMain.on('CellEvent', (event, CellId) => {
    if (!flagGameBegin) {
      return createEmptyArray();
    } else {
      console.log(CellId);
      subProcessOnCPP.stdin.write(CellId + '\n');
      return 0;
    }
  });

  mainWindow = createWindow();

  //On-event. Send array of figures (ChessMassive)
  subProcessOnCPP.stdout.on('data', (data) => {

    console.log(data.toString('utf8'));
    str = data.toString('utf8');

    FiguresArray = makeNecessaryMassive(str);
    SelCellsArray = makeSelCellsArray(str);
    if (FiguresArray == null) {
      mainWindow.webContents.send('update-board', ChessMassive, SelCellsArray);
    } else {
      for (i = 0; i < 64; i++) {
        ChessMassive[i] = FiguresArray[i];
      }
      mainWindow.webContents.send('update-board', ChessMassive);
    }

  });
  //////////////////////////////////////////////////////////////////////
  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})
//Завершение работы программы
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

