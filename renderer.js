function SetFigureOnCell(position, figure, color) {
    elem = document.getElementById(position).firstChild
    if (figure != 'None') {
        elem.className = color + "figure";
        elem.setAttribute("src", "images/" + "B" + figure + ".png");
    } else {
        elem.setAttribute("src", '');
    }

}

//type == 1 - select; type == 0 - no select
function SelectCell(position, type) {
    elem = document.getElementById(position);

    if (type) {
        elem.classList.add("select");
    } else {
        elem.classList.remove("select");
    }
}

function getPositionByIJ(i, j) {
    return String.fromCharCode('a'.charCodeAt(0) + j) + (8 - i)
}

function makeBoardfromMassive(arr) {
    let arr2D = new Array(8);

    for (i = 0; i < 8; i++) {
        arr2D[i] = new Array(8);
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++)
            arr2D[i][j] = arr[8 * i + j];
    }

    return arr2D;
}

function fullBoard(arr) {
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {

            var fullName = arr[i][j]

            if (fullName != 'None') {

                var color = fullName[0]
                var name = fullName.substring(1, fullName.length)
                SetFigureOnCell(getPositionByIJ(i, j), name, color)
            }
            else {
                SetFigureOnCell(getPositionByIJ(i, j), 'None')
            }
        }
}

function SelectAllCells(SelCells) {
    if (SelCells == null) {
        for (i = 0; i < 8; i++)
            for (j = 0; j < 8; j++) {
                SelectCell(getPositionByIJ(i, j), 0);
            }
    } else {
        SelCells.forEach(element => {
            SelectCell(element, 1);     
        });
    }
}

//Send to main event Cell ID
async function Cellhandler() {
    window.electronAPI.CellEvent(this.id);
}

async function handleStartButton() {
    const arr = await window.electronAPI.StartButton();
    arr2D = makeBoardfromMassive(arr);
    fullBoard(arr2D);
}

//On-event. Main send to render board in answer on CellID and out this board
window.electronAPI.UpdateBoard((event, ChessBoard, SelCells) => {
    SelectAllCells(SelCells);
    arr2D = makeBoardfromMassive(ChessBoard);
    fullBoard(arr2D);
});

const WCells = document.querySelectorAll('.white');

WCells.forEach(WCell => {
    WCell.addEventListener('click', Cellhandler);
})

const BCells = document.querySelectorAll('.black');

BCells.forEach(BCell => {
    BCell.addEventListener('click', Cellhandler)
})

const StartButton = document.getElementById('StartButton')

StartButton.addEventListener("click", handleStartButton, { once: true })