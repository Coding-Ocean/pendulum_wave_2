#include"libOne.h"
//１個の振り子
class PENDULUM {
public:
    PENDULUM() {}
    ~PENDULUM() {}
    void create(int i) {
        //１８０秒間に振り子を往復させる回数を指定
        int numRoundTrip = 80 - i;

        //---１８０秒でnumRoundTrip回往復させるためには
        //　１フレームで何度加算すればよいのか---
        //まず、１８０秒で６０往復するとき１フレームで何度動かせばいいかを考える
        //１往復にかける時間：１８０秒÷６０往復＝３秒
        //３秒（１８０フレーム）で１往復するためには
        //３６０度÷１８０フレーム＝２度 per frame
        //２度：６０往復　=　AnglVelo ： numRoundTrip
        AnglVelo = 2.0f * numRoundTrip / 60;

        //---振り子の紐の長さ---
        Length = 1100;

        //振り子の支点位置
        Sx = width / 2;
        Sy = -900 + 20 * i;
        //振り切った時の角度
        MaxDeg = 15;
        //振り子の直径
        Diameter = 20;
        //振り子の色
        Hue = 0; Satulation = 0; Value = 255;
        //最初のおもりの位置
        angleMode(DEGREES);
        Ex = Sx + sin(MaxDeg) * Length;
        Ey = Sy + cos(MaxDeg) * Length;
    }
    void resetAngle() {
        Angle = 0;
    }
    void setColor(float h, float s, float v) {
        Hue = h;
        Satulation = s;
        Value = v;
    }
    void move() {
        angleMode(DEGREES);
        float deg = MaxDeg * cos(Angle);
        Ex = Sx + sin(deg) * Length;
        Ey = Sy + cos(deg) * Length;
        Angle += AnglVelo;
    }
    void draw() {
        colorMode(HSV);
        stroke(80);
        line(Sx, Sy, Ex, Ey);
        fill(Hue, Satulation, Value);
        circle(Ex, Ey, Diameter);
    }
private:
    //色（HSV）
    float Hue = 0, Satulation = 0, Value = 0;
    //振り子の制御に使う角度。この値が360の倍数になると１往復。
    float Angle = 0;
    //１フレームごとにAngleに加算する角速度
    float AnglVelo = 0;
    //振り切った時の角度
    float MaxDeg = 0;
    //振り子の長さ
    float Length = 0;
    //始点と終点（＝支点とおもりの位置）
    float Sx = 0, Sy = 0, Ex = 0, Ey = 0;
    //おもりの直径
    float Diameter = 0;
};

//全ての振り子
class PENDULUMS {
public:
    PENDULUMS() { 
    }
    ~PENDULUMS() {
        if (Pendulums)delete[] Pendulums;
    }
    void create(int num) {
        Num = num;
        Pendulums = new PENDULUM[Num];
        for (int i = 0; i < Num; i++) {
            Pendulums[i].create(i);
        }

        ColorNo = Num;
        for (int i = 0; i < Num; i++) {
            float hue = 360 / ColorNo * i;
            Pendulums[i].setColor(hue, 128, 255);
        }

        Count = 3;
        CountDown = 60*Count;
    }
    void move() {
        //フェードイン
        if (CountDown>0) {
            CountDown--;
        }
        else if(CountDown==0){
            MoveFlag = !MoveFlag;
            CountDown = -1;
        }
        //動き
        if (MoveFlag) {
            //１週したらリセットする（誤差修正）
            if (++FrameCnt >= 3600*3) {
                FrameCnt = 0;
                for (int i = 0; i < Num; i++) {
                    Pendulums[i].resetAngle();
                }
            }
            //動かす
            for (int i = 0; i < Num; i++) {
                Pendulums[i].move();
            }
        }
        //色を変える
        if (isTrigger(KEY_W)) {
            ChangeColor(1);
        }
        if (isTrigger(KEY_S)) {
            ChangeColor(-1);
        }
        //テキスト表示・非表示
        if (isTrigger(KEY_SPACE)) {
            ShowCntFlag = !ShowCntFlag;
        }
    }
    void draw() {
        //描画
        for (int i = 0; i < Num; i++) {
            Pendulums[i].draw();
        }
        if (ShowCntFlag) {
            textSize(40);
            fill(0, 0, 255, 128);
            text("Coding Ocean", width-12*20, height);
        }
        //フェードイン
        if (CountDown > 0) {
            fill(0, 0, 0, 255 * CountDown / (Count * 60.0f));
            rect(0, 0, width, height);
        }
    }
private:
    void ChangeColor(int i) {
        if (i > 0) {
            ++ColorNo;
            if (ColorNo > Num) {
                ColorNo = 0;
            }
        }
        else {
            --ColorNo;
            if (ColorNo < 0) {
                ColorNo = Num;
            }
        }
        float satulation = 120;
        int colorNo = ColorNo;
        if (ColorNo == 0) {
            satulation = 0;
            colorNo = 1;
        }
        for (int i = 0; i < Num; i++) {
            float hue = 360 / colorNo * i;
            Pendulums[i].setColor(hue, satulation, 255);
        }
    }
private:
    PENDULUM* Pendulums = 0;
    int Num = 0;
    int ColorNo = 0;
    int FrameCnt = 0;
    int ShowCntFlag = 0;
    int MoveFlag = 0;
    int CountDown = 0;
    int Count = 0;
};

void gmain() {
    window(1920, 1080, full);
    ShowCursor(FALSE);
    PENDULUMS pendulums;
    pendulums.create(1);
    while (notQuit) {
        pendulums.move();
        clear(10);
        pendulums.draw();
    }
    ShowCursor(TRUE);
}

/*
ペンデュラムウェーブ（振り子の波）
１分間に揺れる回数を８０回、７９回、７８回、７７回・・・・・
となるように調節した複数の振り子を同時に揺らし始めると、
規則性を持った美しい波をつくりだし、
１分後にはまた、最初の位置に整列する。
（ハーバード大が２０１０年に発表した）
*/

/*
現実物理世界では、
おもりの重さにも振れ幅にも関係なく
「ひもの長さ」によって、振り子の周期は決定される。
式：
t = 2π√l/g
　tは一往復する時間
　lは紐の長さ
　gは重力
 （あのガリレオさんによって研究された）

 情報世界では、周期だけ計算しておけば、
 紐の長さはどうでもよい。
*/
