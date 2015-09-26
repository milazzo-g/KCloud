import QtQuick 2.0

Item{

	id: root
	height: 50
	width: 50

	property alias color: btnContainer.color;

	Rectangle{
		id: btnContainer
		anchors.fill: root
		color: "#0099FF"
		Text{
			id: btnName
			color: "white"
			text: "prova"
		}
	}
}

