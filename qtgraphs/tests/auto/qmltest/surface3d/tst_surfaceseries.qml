// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 2.0
import QtGraphs
import QtTest 1.0

Item {
    id: top
    height: 150
    width: 150

    Surface3DSeries {
        id: initial
    }

    Gradient {
        id: gradient1;
        stops: [
            GradientStop { color: "red"; position: 0 },
            GradientStop { color: "blue"; position: 1 }
        ]
    }

    Gradient {
        id: gradient2;
        stops: [
            GradientStop { color: "green"; position: 0 },
            GradientStop { color: "red"; position: 1 }
        ]
    }

    Gradient {
        id: gradient3;
        stops: [
            GradientStop { color: "gray"; position: 0 },
            GradientStop { color: "darkgray"; position: 1 }
        ]
    }

    Surface3DSeries {
        id: initialized
        dataProxy: ItemModelSurfaceDataProxy {
            itemModel: ListModel {
                ListElement{ longitude: "20"; latitude: "10"; pop_density: "4.75"; }
                ListElement{ longitude: "21"; latitude: "10"; pop_density: "3.00"; }
            }
            rowRole: "longitude"
            columnRole: "latitude"
            yPosRole: "pop_density"
        }
        drawMode: Surface3DSeries.DrawSurface
        flatShadingEnabled: false
        selectedPoint: Qt.point(0, 0)
        textureFile: ":\customtexture.jpg"
        wireframeColor: "red"

        baseColor: "blue"
        baseGradient: gradient1
        colorStyle: Theme3D.ColorStyle.ObjectGradient
        itemLabelFormat: "%f"
        itemLabelVisible: false
        mesh: Abstract3DSeries.Mesh.Cube
        meshRotation: Qt.quaternion(1, 1, 1, 1)
        meshSmooth: true
        multiHighlightColor: "green"
        multiHighlightGradient: gradient2
        name: "series1"
        singleHighlightColor: "red"
        singleHighlightGradient: gradient3
        userDefinedMesh: ":/customitem.obj"
        visible: false
    }

    ItemModelSurfaceDataProxy {
        id: proxy1
        itemModel: ListModel {
            ListElement{ longitude: "20"; latitude: "10"; pop_density: "4.75"; }
            ListElement{ longitude: "21"; latitude: "10"; pop_density: "3.00"; }
            ListElement{ longitude: "22"; latitude: "10"; pop_density: "1.24"; }
        }
        rowRole: "longitude"
        columnRole: "latitude"
        yPosRole: "pop_density"
    }

    Surface3DSeries {
        id: change
    }

    TestCase {
        name: "Surface3DSeries Initial"

        function test_1_initial() {
            compare(initial.dataProxy.rowCount, 0)
            compare(initial.invalidSelectionPosition, Qt.point(-1, -1))
            compare(initial.drawMode, Surface3DSeries.DrawSurfaceAndWireframe)
            compare(initial.flatShadingEnabled, true)
            compare(initial.flatShadingSupported, true)
            compare(initial.selectedPoint, Qt.point(-1, -1))
            compare(initial.wireframeColor, "#000000")
        }

        function test_2_initial_common() {
            // Common properties
            compare(initial.baseColor, "#000000")
            compare(initial.baseGradient, 0)
            compare(initial.colorStyle, Theme3D.ColorStyle.Uniform)
            compare(initial.itemLabel, "")
            compare(initial.itemLabelFormat, "@xLabel, @yLabel, @zLabel")
            compare(initial.itemLabelVisible, true)
            compare(initial.mesh, Abstract3DSeries.Mesh.Sphere)
            compare(initial.meshRotation, Qt.quaternion(1, 0, 0, 0))
            compare(initial.meshSmooth, false)
            compare(initial.multiHighlightColor, "#000000")
            compare(initial.multiHighlightGradient, 0)
            compare(initial.name, "")
            compare(initial.singleHighlightColor, "#000000")
            compare(initial.singleHighlightGradient, 0)
            compare(initial.type, Abstract3DSeries.SeriesType.Surface)
            compare(initial.userDefinedMesh, "")
            compare(initial.visible, true)
        }
    }

    TestCase {
        name: "Surface3DSeries Initialized"

        function test_1_initialized() {
            compare(initialized.dataProxy.rowCount, 2)
            compare(initialized.drawMode, Surface3DSeries.DrawSurface)
            compare(initialized.flatShadingEnabled, false)
            compare(initialized.selectedPoint, Qt.point(0, 0))
            compare(initialized.textureFile, ":\customtexture.jpg")
            compare(initialized.wireframeColor, "#ff0000")
        }

        function test_2_initialized_common() {
            // Common properties
            compare(initialized.baseColor, "#0000ff")
            compare(initialized.baseGradient, gradient1)
            compare(initialized.colorStyle, Theme3D.ColorStyle.ObjectGradient)
            compare(initialized.itemLabelFormat, "%f")
            compare(initialized.itemLabelVisible, false)
            compare(initialized.mesh, Abstract3DSeries.Mesh.Cube)
            compare(initialized.meshRotation, Qt.quaternion(1, 1, 1, 1))
            compare(initialized.meshSmooth, true)
            compare(initialized.multiHighlightColor, "#008000")
            compare(initialized.multiHighlightGradient, gradient2)
            compare(initialized.name, "series1")
            compare(initialized.singleHighlightColor, "#ff0000")
            compare(initialized.singleHighlightGradient, gradient3)
            compare(initialized.userDefinedMesh, ":/customitem.obj")
            compare(initialized.visible, false)
        }
    }

    TestCase {
        name: "Surface3DSeries Change"

        function test_1_change() {
            change.dataProxy = proxy1
            change.drawMode = Surface3DSeries.DrawSurface
            change.flatShadingEnabled = false
            change.selectedPoint = Qt.point(0, 0)
            change.textureFile = ":\customtexture.jpg"
            change.wireframeColor = "green"
        }

        function test_2_test_change() {
            // This test has a dependency to the previous one due to asynchronous item model resolving
            compare(change.dataProxy.rowCount, 3)
            compare(change.drawMode, Surface3DSeries.DrawSurface)
            compare(change.flatShadingEnabled, false)
            compare(change.selectedPoint, Qt.point(0, 0))
            compare(change.textureFile, ":\customtexture.jpg")
            compare(change.wireframeColor, "#008000")
        }

        function test_3_change_common() {
            change.baseColor = "blue"
            change.baseGradient = gradient1
            change.colorStyle = Theme3D.ColorStyle.ObjectGradient
            change.itemLabelFormat = "%f"
            change.itemLabelVisible = false
            change.mesh = Abstract3DSeries.Mesh.Cube
            change.meshRotation = Qt.quaternion(1, 1, 1, 1)
            change.meshSmooth = true
            change.multiHighlightColor = "green"
            change.multiHighlightGradient = gradient2
            change.name = "series1"
            change.singleHighlightColor = "red"
            change.singleHighlightGradient = gradient3
            change.userDefinedMesh = ":/customitem.obj"
            change.visible = false

            compare(change.baseColor, "#0000ff")
            compare(change.baseGradient, gradient1)
            compare(change.colorStyle, Theme3D.ColorStyle.ObjectGradient)
            compare(change.itemLabelFormat, "%f")
            compare(change.itemLabelVisible, false)
            compare(change.mesh, Abstract3DSeries.Mesh.Cube)
            compare(change.meshRotation, Qt.quaternion(1, 1, 1, 1))
            compare(change.meshSmooth, true)
            compare(change.multiHighlightColor, "#008000")
            compare(change.multiHighlightGradient, gradient2)
            compare(change.name, "series1")
            compare(change.singleHighlightColor, "#ff0000")
            compare(change.singleHighlightGradient, gradient3)
            compare(change.userDefinedMesh, ":/customitem.obj")
            compare(change.visible, false)
        }

        function test_4_change_gradient_stop() {
            gradient1.stops[0].color = "yellow"
            compare(change.baseGradient.stops[0].color, "#ffff00")
        }
    }
}
