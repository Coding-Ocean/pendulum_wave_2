#include"libOne.h"
//�P�̐U��q
class PENDULUM {
public:
    PENDULUM() {}
    ~PENDULUM() {}
    void create(int i) {
        //�P�W�O�b�ԂɐU��q������������񐔂��w��
        int numRoundTrip = 80 - i;

        //---�P�W�O�b��numRoundTrip�񉝕������邽�߂ɂ�
        //�@�P�t���[���ŉ��x���Z����΂悢�̂�---
        //�܂��A�P�W�O�b�łU�O��������Ƃ��P�t���[���ŉ��x�������΂��������l����
        //�P�����ɂ����鎞�ԁF�P�W�O�b���U�O�������R�b
        //�R�b�i�P�W�O�t���[���j�łP�������邽�߂ɂ�
        //�R�U�O�x���P�W�O�t���[�����Q�x per frame
        //�Q�x�F�U�O�����@=�@AnglVelo �F numRoundTrip
        AnglVelo = 2.0f * numRoundTrip / 60;

        //---�U��q�̕R�̒���---
        Length = 1100;

        //�U��q�̎x�_�ʒu
        Sx = width / 2;
        Sy = -900 + 20 * i;
        //�U��؂������̊p�x
        MaxDeg = 15;
        //�U��q�̒��a
        Diameter = 20;
        //�U��q�̐F
        Hue = 0; Satulation = 0; Value = 255;
        //�ŏ��̂�����̈ʒu
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
    //�F�iHSV�j
    float Hue = 0, Satulation = 0, Value = 0;
    //�U��q�̐���Ɏg���p�x�B���̒l��360�̔{���ɂȂ�ƂP�����B
    float Angle = 0;
    //�P�t���[�����Ƃ�Angle�ɉ��Z����p���x
    float AnglVelo = 0;
    //�U��؂������̊p�x
    float MaxDeg = 0;
    //�U��q�̒���
    float Length = 0;
    //�n�_�ƏI�_�i���x�_�Ƃ�����̈ʒu�j
    float Sx = 0, Sy = 0, Ex = 0, Ey = 0;
    //������̒��a
    float Diameter = 0;
};

//�S�Ă̐U��q
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
        //�t�F�[�h�C��
        if (CountDown>0) {
            CountDown--;
        }
        else if(CountDown==0){
            MoveFlag = !MoveFlag;
            CountDown = -1;
        }
        //����
        if (MoveFlag) {
            //�P�T�����烊�Z�b�g����i�덷�C���j
            if (++FrameCnt >= 3600*3) {
                FrameCnt = 0;
                for (int i = 0; i < Num; i++) {
                    Pendulums[i].resetAngle();
                }
            }
            //������
            for (int i = 0; i < Num; i++) {
                Pendulums[i].move();
            }
        }
        //�F��ς���
        if (isTrigger(KEY_W)) {
            ChangeColor(1);
        }
        if (isTrigger(KEY_S)) {
            ChangeColor(-1);
        }
        //�e�L�X�g�\���E��\��
        if (isTrigger(KEY_SPACE)) {
            ShowCntFlag = !ShowCntFlag;
        }
    }
    void draw() {
        //�`��
        for (int i = 0; i < Num; i++) {
            Pendulums[i].draw();
        }
        if (ShowCntFlag) {
            textSize(40);
            fill(0, 0, 255, 128);
            text("Coding Ocean", width-12*20, height);
        }
        //�t�F�[�h�C��
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
�y���f�������E�F�[�u�i�U��q�̔g�j
�P���Ԃɗh���񐔂��W�O��A�V�X��A�V�W��A�V�V��E�E�E�E�E
�ƂȂ�悤�ɒ��߂��������̐U��q�𓯎��ɗh�炵�n�߂�ƁA
�K�������������������g�����肾���A
�P����ɂ͂܂��A�ŏ��̈ʒu�ɐ��񂷂�B
�i�n�[�o�[�h�傪�Q�O�P�O�N�ɔ��\�����j
*/

/*
�����������E�ł́A
������̏d���ɂ��U�ꕝ�ɂ��֌W�Ȃ�
�u�Ђ��̒����v�ɂ���āA�U��q�̎����͌��肳���B
���F
t = 2�΁�l/g
�@t�͈ꉝ�����鎞��
�@l�͕R�̒���
�@g�͏d��
 �i���̃K�����I����ɂ���Č������ꂽ�j

 ��񐢊E�ł́A���������v�Z���Ă����΁A
 �R�̒����͂ǂ��ł��悢�B
*/
