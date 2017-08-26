import QtQuick 2.0

Rectangle {
    id: back
    color: "red"

    Canvas {
        anchors.fill: parent
        width: back.width
        height: back.height

        onPaint: {
            var ctx = getContext("2d")
            ctx.fillStyle = 'blue'

            var centerX = width/2
            var centerY = height/2
            var radius = Math.min(width/3, height/3)

            var str1 = height/2
            var str2 = "px sans-serif"
            ctx.font = str1.toString().concat(str2)
            ctx.textAlign = "center"
            ctx.fillText("DONE", centerX, centerY + 10)
        }
    }

}