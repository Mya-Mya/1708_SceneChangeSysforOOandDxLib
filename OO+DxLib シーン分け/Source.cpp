#include"DxLib.h"

/*
�d�l
A�AB�AC�̂R�̃V�[��������B���ꂼ��A�AB�AC�L�[���������ƂŕύX�ł���B

���[��
DxLib�ɍœK�ȃI�u�W�F�N�g�w���ł̃V�[���Ǘ��̊�ՃR�[�h : 1�b������60�񃁃C���T�u���[�`�����Ăяo���B���̓s�x��ʂ̍ĕ`����s���B

Change : �V�[���`�F���W�N���X �C���^�[�t�F�[�X�̖���
Mgr : �V�[���Ǘ��N���X �e���W���[���� Base* �ŏ��L���邱�ƁA�������R���X�g���N�^�ɂĎ����̃|�C���^(this)��n������ �܂� Change���p�����邱��
Base : ���W���[���̒��ۃN���X
�e���W���[���N���X : Base ���p�����邱�� �܂��R���X�g���N�^�ɂ� Change* �Ƃ��ă|�C���^���L�����邱��

�e���W���[���̓��얽�߂̊֐��� �e���W���[��::RUN()
�V�[���`�F���W���߂� Change::DO()
���C���T�u���[�`���֐��� Mgr::ROOT()

���W���[���� Module_A , Module_B , Module_C ��3��
�V�[�����͗񋓌^eScene�ɂĎ����BScene_A -> Module_A , Scene_B -> Module_B , Scene_C -> Module_C
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