#include"DxLib.h"

/*
仕様
A、B、Cの３つのシーンがある。それぞれA、B、Cキーを押すことで変更できる。

ルール
DxLibに最適なオブジェクト指向でのシーン管理の基盤コード : 1秒あたり60回メインサブルーチンを呼び出す。その都度画面の再描画を行う。

Change : シーンチェンジクラス インターフェースの役割
Mgr : シーン管理クラス 各モジュールを Base* で所有すること、生成時コンストラクタにて自分のポインタ(this)を渡すこと また Changeを継承すること
Base : モジュールの抽象クラス
各モジュールクラス : Base を継承すること またコンストラクタにて Change* としてポインタを記憶すること

各モジュールの動作命令の関数は 各モジュール::RUN()
シーンチェンジ命令は Change::DO()
メインサブルーチン関数は Mgr::ROOT()

モジュールは Module_A , Module_B , Module_C の3つ
シーン情報は列挙型eSceneにて示す。Scene_A -> Module_A , Scene_B -> Module_B , Scene_C -> Module_C
*/

typedef enum {
	Scene_A,
	Scene_B,
	Scene_C,
	Scene_None,
}eScene;

class Change {
protected:
	eScene next = Scene_A;
public:
	Change() {}
	virtual void DO(eScene in) { next = in; }
};

class Base {
protected:
	Change *pChange = NULL;
public:
	Base(Change *pIn) { pChange = pIn; }
	virtual void RUN() = 0;
};

class Module_A :public Base{
public:
	Module_A(Change *pIn) :Base(pIn) {  }
	void RUN() override {
		DrawFormatString(0, 0, GetColor(255, 200, 255), "Module_A. B->Module B C->Module_C");
		if (CheckHitKey(KEY_INPUT_B) == 1)pChange->DO(Scene_B);
		if (CheckHitKey(KEY_INPUT_C) == 1) pChange->DO(Scene_C);
	}
};
class Module_B:public Base {
public:
	Module_B(Change *pIn):Base(pIn){}
	void RUN() override {
		DrawFormatString(0, 0, GetColor(255, 255, 200), "Module_B. A->Module A C->Module_C");
		if (CheckHitKey(KEY_INPUT_A) == 1) pChange->DO(Scene_A);
		if (CheckHitKey(KEY_INPUT_C) == 1)pChange->DO(Scene_C);
	}
};
class Module_C:public Base {
public:
	Module_C(Change *pIn):Base(pIn){}
	void RUN() override {
		DrawFormatString(0, 0, GetColor(200, 255, 255), "Module_C. A->Module A B->Module_B");
		if (CheckHitKey(KEY_INPUT_A) == 1) pChange->DO(Scene_A);
		if (CheckHitKey(KEY_INPUT_B) == 1)pChange->DO(Scene_B);
	}
};

class Mgr :public Change{
private:
	Base *pBase = NULL;
public:
	Mgr(){}
	void ROOT() {
		if (next != Scene_None) {
			delete pBase;
			switch (next){
			case Scene_A:
				pBase = (Base*)new Module_A(this);
				break;
			case Scene_B:
				pBase = (Base*)new Module_B(this);
				break;
			case Scene_C:
				pBase = (Base*)new Module_C(this);
				break;
			}
			next = Scene_None;
		}
		pBase->RUN();		
	}
};

int main() {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawMode(DX_SCREEN_BACK);

	Mgr mgr;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		mgr.ROOT();
	}

	DxLib_End();
	return 0;
}