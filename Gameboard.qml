import QtQuick 2.0
import Game 1.0

GridView {
    id: root
    model: GameboardModel{

    }


    header: Rectangle {
       id: headerId
        height: 50
        width:parent.width
        color: "red"
        Text{
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5
        font.pointSize: 15
        text: "Numbrer of move "+root.model.moveNumber
        color:"blue"
        }
        Text{
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 5
        font.pointSize: 15
        text: "Time "+time.i
        color:"blue"
        }
        Timer {
              id:time
              property int i:0
              interval: 1000
              repeat: true
              running: true
              onTriggered: {
                 i++
              }
        }
    }



    cellHeight: (height-headerItem.height)/root.model.dimension
    cellWidth: width/root.model.dimension
    delegate: Item {
        id: bgDelegate
        width: root.cellWidth
        height: root.cellHeight
        visible: display !== root.model.hiddenElementValue
        Tile{
        anchors.fill: bgDelegate
        anchors.margins: 5
        displayText: display

        MouseArea {
            anchors.fill: parent
            onClicked: {
               root.model.move(index)
                if(checkMoving()){
                   var dir = checkDirection()
                    if(dir === 1){
                        left.start()
                        console.log("left")
                        console.log(bgDelegate.x)
                    } else if(dir === 2){
                        right.start()
                        console.log("right")
                        console.log(bgDelegate.x)
                    } else if (dir === 3){
                        up.start()
                        console.log("up")
                        console.log(bgDelegate.y)
                    } else if (dir === 4){
                        down.start()
                        console.log("down")
                        console.log(bgDelegate.y)
                    }
                }
            }
            NumberAnimation{
                id: right
                target: bgDelegate
                property: "x"
                duration: 200
                from: bgDelegate.x
                to: bgDelegate.x+bgDelegate.width
            }
            NumberAnimation{
                id: left
                target: bgDelegate
                property: "x"
                duration: 200
                from: bgDelegate.x
                to: bgDelegate.x-bgDelegate.width
            }
            NumberAnimation{
                id: up
                target: bgDelegate
                property: "y"
                duration: 200
                from: bgDelegate.y
                to: bgDelegate.y-bgDelegate.height
            }
            NumberAnimation{
                id: down
                target: bgDelegate
                property: "y"
                duration: 200
                from: bgDelegate.y
                to: bgDelegate.y+bgDelegate.height
            }
            function checkMoving(){
                  return root.model.moveble
            }

            function checkDirection(){
                return root.model.direct
            }

        }

    }

    }

}

