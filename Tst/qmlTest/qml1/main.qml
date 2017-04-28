import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import armadillo 1.0

ApplicationWindow {
    visible: true
    width: 750
    height: 600
    title: qsTr("Hello World")

    QtObject {
        id: internal
        readonly property var dir: "../RawData/Wilkinson/"
        readonly property var fileA53_o: dir+"Gilbert7.txt"
        readonly property var fileDetA53_o: dir+"det_A53_o.txt"
        readonly property var fileInvA53_o: dir+"invA53.prn"
        readonly property var fileD_p47_o: dir+"d_p47_o.prn"
        readonly property var fileM_p56_o: dir+"M_p56_o.prn"

        property var a53_o: MatrArm {}
        property var detA53_o: MatrArm {}
        property var invA53_t: MatrArm {}
        property var invA53_o: MatrArm {}
        property var d_p47_o: MatrArm {}
        property var m_p56_o: MatrArm {}
        property var l_p56_o: MatrArm {}
        property var u_p56_o: MatrArm {}

        property var aHlpr: ArmaHlpr {}
    }

    function scienceFormat(value) {
        return value.toExponential(5);
    }

    function relErr(tv, ov) {
        return scienceFormat(Math.abs(tv-ov)/Math.abs(ov));
    }

    function testCholeckij_p61() {
        const n = 40;
        function genGDiag(size) {
            var res = internal.aHlpr.mkVecByDig(size, 6);
            res.setElem(0, 0, 5);
            res.setElem(size-1, 0, 5);
            return res;
        }
        function ftDiag(size) {
            return internal.aHlpr.mkVecByDig(size, -4);
        }
        function sdDiag(size) {
            return internal.aHlpr.mkVecByDig(size, 1);
        }
        //
        function indexTransformGD(i) {
            return [[i,i]];
        }
        function indexTransformFirstDiag(i) {
            return [
                        [i, i+1],
                        [i+1, i]
                   ];
        }
        function indexTransformSecondDiag(i) {
            return [
                        [i, i+2],
                        [i+2, i]
                   ];
        }
        //
        var gd = genGDiag(n);
        var fd = ftDiag(n-1);
        var sd = sdDiag(n-2);
        //
        function putVectorToMatr(vec, TransfomRule, matr) {
            var size = vec.rows();
            for(var i = 0; i<size; i++) {
                var indx = TransfomRule(i);
                for (var j=0; j< indx.length; j++) {
                    //console.warn("indx="+indx);
                    var r = indx[j][0];
                    var c = indx[j][1];
                    matr.setElem(r, c, vec.getElem(i, 0));
                }
            }
        }
        //
        var res = internal.aHlpr.mkMatr(n, n);
        putVectorToMatr(gd, indexTransformGD, res);
        putVectorToMatr(fd, indexTransformFirstDiag, res);
        putVectorToMatr(sd, indexTransformSecondDiag, res);
        //
        //console.warn("QQ_="+res.print("A_p61"));
        var det_A_p61_t = res.det();
        console.warn("det="+det_A_p61_t);
        //
        var a1 = MatrArm;
        a1.readMatrFromFile(internal.dir+"d_A_p61_o.prn");
        console.warn("det_t="+a1.det());
        //
        return res;
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page {
            Flickable {
                anchors.fill: parent
                //contentWidth: parent.width * 2
                contentHeight: parent.height * 2

                ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
                ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }

            ColumnLayout {
                id: column
                RowLayout {
                    Label {
                        text: "Armadillo TEST"
                    }
                    Label {
                        text: "ReadMatrixFromFile: " + internal.fileA53_o
                    }
                }
                Button {
                    text: "Start Load"
                    onClicked: {
                        internal.a53_o.readMatrFromFile(internal.fileA53_o);
                        internal.detA53_o.readMatrFromFile(internal.fileDetA53_o);
                        //
                        textArea2.text = internal.a53_o.print("Gilb7");
                        //
                        var strDet_A53_o = internal.detA53_o.toStr();
                        var strDet_A53_t = internal.a53_o.det()
                        var relE =relErr(strDet_A53_t, strDet_A53_o);
                        det.text = "relErr(det(A53))="+relE;
                        //
                        internal.invA53_t = internal.a53_o.inv2();
                        textArea3.text = internal.invA53_t.print("inv(Gilb7)");
                        internal.invA53_o.readMatrFromFile(internal.fileInvA53_o);
                        textInvA53o.text = internal.invA53_o.print("inv(Gilb7)_original");
                        //
                        var a1 = internal.invA53_o.getDelta(internal.invA53_t);
                        minmaxDeltaMatr.text = "matVariation(invA53)={"+
                                                scienceFormat(a1[0])+", "+
                                                scienceFormat(a1[1])+"}";
                        //
                        var vec1 = internal.aHlpr.mkVec_e(7,0);
                        var d_p47_t = internal.a53_o.solve(vec1);
                        internal.d_p47_o.readMatrFromFile(internal.fileD_p47_o);
                        //
                        var delta_D_p47 = d_p47_t.getDelta(internal.d_p47_o);
//                        console.warn("QQQ_matVariation(delta_D_p47)={"+
//                                                scienceFormat(delta_D_p47[0])+", "+
//                                                scienceFormat(delta_D_p47[1])+"}");
                        //
                        internal.m_p56_o.readMatrFromFile(internal.fileM_p56_o);
//                        console.warn("QQ_="+internal.m_p56_o.print("M_p56_o"));
                        //LU
                        internal.m_p56_o.lu(internal.l_p56_o, internal.u_p56_o);
//                        console.warn("QQ_="+internal.l_p56_o.print("L_p56_o"));
//                        console.warn("QQ_="+internal.u_p56_o.print("U_p56_o"));
                        //
                        var A_p61 = testCholeckij_p61();
                        //console.warn("QQ_="+A_p61.print("A_p61"));
                    }
                }
                TextArea {
                    id: textArea2
                    font.pointSize: 13
                    width: parent.width
                    text: "empty"
                }
                Label {
                    id: det
                    font.pointSize: 13
                    text: "null"
                }
                TextArea {
                    id: textArea3
                    font.pointSize: 13
                    width: parent.width
                    text: "empty"
                }
                TextArea {
                    id: textInvA53o
                    font.pointSize: 13
                    width: parent.width
                    text: "empty"
                }
                Label {
                    id: minmaxDeltaMatr
                    font.pointSize: 13
                    text: "null"
                }
            }
        }
        }

        Page1 { }
        Page {
            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("3")
        }
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }
}
