#include <SFML/Graphics.hpp> //�������� ���������� ��� ������ � �������� � ������������
#include <SFML/Audio.hpp> //��� ������ �� ������ � �������
#include <iostream> //��� ������ � �����
#include <fstream> //��� ������ � ������� (������������ ���� ������ ���� �� ������ � �.�.)
#include "forMcText.h" //���� ���������� �� ������ kychka-pc.ru/wiki/svobodnaya-baza-znanij-sfml/test/sistemnyj-modul-system-module/potoki-threads/sfsound-sfmusic
#include "searchWay.h" //���������� ���� ������� ��� ���������� ������������ ���� � ��������� (����� � ������) ��� �������� �����
#include "view.h" //���������� �������
#include "system.h" //�������
#include "body.h" //���� �������
#include "general classes.h" //�������� ������
#include "quickSort.h" //������� ����������
#include "binSearch.h" //�������� �����
using namespace std;
using namespace sf;


//�������������� ��, ����� ��� � ���� ���������� �������� ���-��, �� ��� �������� � �� ���� ������� �����������
bool         System::F_gameIsLocked; //����, ������� ���������� ������������� �� ���� (���� �������� ���������)
bool         System::F_musicOff; //����, ������� ���������� ��������� �� ������
bool         System::F_soundOff; //����, ������� ���������� ��������� �� �����
bool         System::F_lvlComplete; //����, ������� ���������� �������� �� �������
bool         System::F_changeStates; //����, ������� ����������, �������� �� ��������� � ������ ������

float        System::speedChangeSt; //�������� ��������� ������� ������ ��� ��������� ���������
float        System::speed; //�������� � ������� �������� ����� �� ������
float        System::timer; //������
float        System::time; //�����
float        System::volumeMusic; //���������� ������� ������
float        System::volumeSound; //��������� �����

int          System::FPS; //�������� ���
int          System::keyCodePressed; //����� ������� ���� ������
int          System::key [3]; //������ � ������� �������� ������ ������, ������� ������ ������
int          System::whatButChange; //��� ������� �������, ����� ������ ������� ��� �������� (����� ����� ����������� ������� ��������� ������)
int          System::passedLVL; //������� �������� �������
int*         System::levelArray; //������ � ������� �������� �����

RenderWindow *System::window; //����, � ������� ����������� ����
Event        System::event; //�������
Clock        System::clock; //�����
Sound        System::sndTeleport; //���� ��������� ������ � ����������
Sound        System::sndClickButt; //���� ������� �� ������
Music        System::backgroundMusic; //������� ������
SoundBuffer  System::bufferClickButt; //����� ��� ����� ������� �� ������
SoundBuffer  System::bufferTeleport; //����� ��� ����� ��������� ������ � ����������
String       System::AdOrPlMode; //������ �������� ��� �������� ���� ���� (����� ��� �����)
Vector2i     System::mousePosWin; //���������� ���� �����. ����
Vector2f     System::posMouse; //���������� ���� �����. �����
TileMap      System::map; //����� (������� ����� �����)
CreateWall   System::whichWall; //����� ����� ������� ������� � ������ ������
Coordinate   System::Start; //���������� ������ (������ ����� ��������) 
Coordinate   System::Finish; //���������� ������ (���� ������ ������)


class Button : public BodyButton{ //������� ������
private:
	bool F_locked; //����, ������� ���������� ������������� �� ������
public:
	Button (Texture *tmpTexture, String Text, String Name, Font &Font, SubtypeState &Subtype, int X, int Y, int W, int H, int Value, int WTexture, int HTexture) : 
		    BodyButton (tmpTexture, Text, Name, Font, Subtype, X, Y, W, H, WTexture, HTexture){
		value = Value; F_locked = false;

		if (subtype == menu) //������ � ����� ���� �������� �����
			F_draw = true;
		else
			F_draw = false;
		shape.setTextureRect (IntRect (0, 0, wTexture, hTexture));
		if (name == "SelectLVL")
			shape.setTextureRect (IntRect (0, 120, wTexture, hTexture));
	}

	void draw (){
		window -> draw (shape);
		text -> draw (window);
	}

	void checkCursor (){ //������� �������� �� ������� ������ ��� ���������� ������� �� ������
		if (name == "SelectLVL")
			if (value > passedLVL + 1)    F_locked = true;
			else                          F_locked = false;
		if (name == "My lvls")
			if (passedLVL >= NUM_LVL - 2) F_locked = false;
			else                          F_locked = true;

		F_click = false;
		if (!F_locked){
			if ((posMouse.x >= x - w / 2) && (posMouse.x <= x + w / 2) && (posMouse.y >= y - h / 2) && (posMouse.y <= y + h / 2)){ //���� ������ ���� ��������� �� ������
				if (Mouse::isButtonPressed (Mouse::Left)) //� ���� ������ �� ���
					F_pressed = true;
				else{
					if (F_pressed) //���� �� ������ �� ������ � ������ ���� ������, � ������ �� ������-������ �� �������� �� ���
						F_click = true; 
					F_pressed = false;
				}
				shape.setTextureRect (IntRect (0, hTexture, wTexture, hTexture)); //���� ������� ������ �� �����, �� ������ ������ ��������
				if (name == "SelectLVL")
					shape.setTextureRect (IntRect (wTexture, 4 * hTexture, wTexture, hTexture));
			}
			else{ //���� ������ �� �� ������
				F_pressed = false; //���� ������ �� �� ���� �� ������ �������� ����
				shape.setTextureRect (IntRect (0, 0, wTexture, hTexture));
				if (name == "SelectLVL")
					shape.setTextureRect (IntRect (0, 4 * hTexture, wTexture, hTexture));
			}
		}
		else{ //���� ������ �������������
			shape.setTextureRect (IntRect (0, hTexture * 3, wTexture, hTexture));
			if (name == "SelectLVL")
				shape.setTextureRect (IntRect (3 * wTexture, 4 * hTexture, wTexture, hTexture));
		}

		if (name == AdOrPlMode) //���� ��� ���������� ������� ������ ��� ������ ������� � �������, �� ������ ������� (�������������� ����������)
			shape.setTextureRect (IntRect (0, hTexture * 2, wTexture, hTexture));
	}

	void updateText (char *Pass){ }
};

class EditButton : public BodyButton{ //������ � ������� ����� ���-�� ������
public:
	EditButton (Texture *tmpTexture, String Text, String Name, Font &Font, SubtypeState &Subtype, int X, int Y, int W, int H, int WTexture, int HTexture) : 
		    BodyButton (tmpTexture, Text, Name, Font, Subtype, X, Y, W, H, WTexture, HTexture){
		if (name == "ChangeKey")
			shape.setTextureRect (IntRect (0, 120, wTexture, hTexture));
		if (Text == "1")      value = 1;
		else if (Text == "2") value = 2;
		else if (Text == "3") value = 3;
	}

	void draw (){
		window -> draw (shape);
		text -> draw (window);
	}

	void checkCursor (){ //������� �������� �� ������� ������ ��� ��������� ������� �� ������
		F_click = false;
		if ((posMouse.x >= x - w / 2) && (posMouse.x <= x + w / 2) && (posMouse.y >= y - h / 2) && (posMouse.y <= y + h / 2)){ //���� ������ ���� ��������� �� ������
			if (Mouse::isButtonPressed (Mouse::Left)) //� ���� ������ �� ���
				F_pressed = true;
			else{
				if (F_pressed) //���� �� ������ �� ������ � ������ ���� ������, � ������ �� ������-������ �� �������� �� ���
					F_click = true; 
				F_pressed = false;
			}
			shape.setTextureRect (IntRect (0, hTexture, wTexture, hTexture)); //���� ������� ������ �� �����, �� ������ ������ ��������
			if (name == "ChangeKey")
				shape.setTextureRect (IntRect (wTexture, 4 * hTexture, wTexture, hTexture));
		}
		else{
			F_pressed = false; //���� ������ �� �� ���� �� ������ �������� ����
			shape.setTextureRect (IntRect (0, 0, wTexture, hTexture));
			if (name == "ChangeKey")
				shape.setTextureRect (IntRect (0, 4 * hTexture, wTexture, hTexture));
		}
		
		if (name == "ChangeKey" && whatButChange == value)
			shape.setTextureRect (IntRect (2 * wTexture, 4 * hTexture, wTexture, hTexture));
	}

	void updateText (char *Pass){ //������� ���������� ������
		buttText = Pass;
		text -> clear ();
		text -> changeSize (sizeText); //������ ������
		text -> add (buttText);
		text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText / 3); //����������� ����� �� ������
	}
};

class Static : public BodyButton{
public:
	Static (String Text, String Name, Font &Font, SubtypeState &Subtype, int X, int Y, Color ccolor) : //���������� ��� ������� ����� ������
		    BodyButton (Text, Name, Font, Subtype, X, Y){ 
		text -> clear ();
		text -> changeSize (sizeText); //������ ������
		color = ccolor;
		text -> add (buttText, color);
		text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText / 3); //����������� ����� �� ������

		if (subtype == wholeType)
			type = player;
	}

	Static (String Text, String Name, Font &Font, SubtypeState &Subtype, int X, int Y) : //�� ��������� ���� ������ ����� ������
		    BodyButton (Text, Name, Font, Subtype, X, Y){ 
		color = Color (0, 0, 0);
		if (subtype == wholeType)
			type = player;
	}

	Static (String Text, String Name, Font &Font, SubtypeState &Subtype, int X, int Y, int tmpSize) : //���������� ��� �������� ������� ������
		    BodyButton (Text, Name, Font, Subtype, X, Y){ 
		sizeText = tmpSize;
		color = Color (0, 0, 0);
	}

	void draw (){
		text -> draw (window);
	}

	void EFF_reduce (){ //���������� ���������� ������, ������ ��� � ������� ��� �����
		if (F_transformation){
			text -> clear ();
			text -> changeSize (sizeText * (int) reducePrecent / 100); //������ ������
			text -> add (buttText, color);
			text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText * reducePrecent / 100 / 3); //����������� ����� �� ������

			reducePrecent -= speedChangeSt * time; //����� ����������� ��������� �����
			if (reducePrecent < speedChangeSt * time){
				reducePrecent = 100;
				F_transformation = false;
			}
		}
	}

	void EFF_enlarge (){ //���������� ���������� ������, ������ ��� � ������� ��� �����
		if (F_transformation){
			text -> clear ();
			text -> changeSize (sizeText * (int) enlargePrecent / 100); //������ ������
			text -> add (buttText, color);
			text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText * enlargePrecent / 100 / 3); //����������� ����� �� ������

			enlargePrecent += speedChangeSt * time; //����� ����������� ��������� �����
			if (enlargePrecent > 100 - speedChangeSt * time){
				enlargePrecent = 1;
				F_transformation = false;

				text -> clear (); //�������� � 100% �������
				text -> changeSize (sizeText); //������ ������
				text -> add (buttText, color);
				text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText / 3); //����������� ����� �� ������
			}
		}
	}

	void updateText (char *Pass){ //���� ���������� ����� ���� ��� ������ ���� �������� �� ������ (��� � � �������� ������� ������ ���� ����)
		text -> clear (); 
		text -> changeSize (sizeText); //������ ������
		char tmpC [40];

		if (name == "TimePlayer")
			strcpy (tmpC, "Time: ");
		else if (name == "FPS")
			strcpy (tmpC, "FPS: ");
		else if (name == "DeathPlayer")
			strcpy (tmpC, "Death: ");
		else if (name == "ControlRec")
			strcpy (tmpC, "Rectangle: ");
		else if (name == "ControlTri")
			strcpy (tmpC, "Triangle: ");
		else if (name == "ControlCir")
			strcpy (tmpC, "Circle: ");

		strcat (tmpC, Pass);
		text -> add (tmpC, color);
		buttText = tmpC;
		text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText / 3); //����������� ����� �� ������
	}

	void checkCursor (){ }
};

class HorizontScrollBar : public BodyButton{ //�������������� ����� ���, ������������ ������ � ���������� ������ � ������
private:
	int leftBorder, rightBorder; //����� � ������ ������� �� ������� ����� ������������ ������ (�� ��� ��������� �������, �.�. ������ ����� ������)
	RectangleShape backgroundd; //���� ����� (�� ������� ��������� ��������)
	int wBground; //������ �����
	int hBground; //������
	int x2, y2; //���������� �����
public:
	HorizontScrollBar (Texture *tmpTexture, String Name, SubtypeState &Subtype, int X, int Y, int W, int H, int tmpBordL, int tmpBordR, int WTexture, int HTexture, int WBground, int HBground) : 
		    BodyButton (tmpTexture, Name, Subtype, X, Y, W, H, WTexture, HTexture){ 
        leftBorder = tmpBordL; rightBorder = tmpBordR;
		wBground = WBground; hBground = HBground;
		x2 = x; y2 = y;

		backgroundd.setSize (Vector2f ((float) W_BUTTON, (float) H_BUTTON));
		backgroundd.setOrigin ((float) W_BUTTON / 2, (float) H_BUTTON / 2);
		backgroundd.setPosition ((float) x2, (float) y2);
		backgroundd.setTexture (tmpTexture);
		backgroundd.setTextureRect (IntRect (0, 5 * hBground, wBground, hBground));	

		shape.setTextureRect (IntRect (0, 4 * hTexture, wTexture, hTexture));
		if (name == "MusicSlider"){ //��������� � ����� ������ �� ���������� ������, � ������������� �������� � ������ ��������
			shape.setPosition ((float) leftBorder + volumeMusic * (rightBorder - leftBorder) / 100, (float) y);
			x = leftBorder + (int) volumeMusic * (rightBorder - leftBorder) / 100;
		}
		if (name == "SoundSlider"){
			shape.setPosition ((float) leftBorder + volumeSound * (rightBorder - leftBorder) / 100, (float) y);
			x = leftBorder + (int) volumeSound * (rightBorder - leftBorder) / 100;
		}
	}

	void draw (){
		window -> draw (backgroundd);
		window -> draw (shape);
	}

	void checkCursor (){ 
		if (posMouse.x >= leftBorder && posMouse.x <= rightBorder) //���� ����� ��������� � ��������� ��� �������� �����
			if (posMouse.y >= y - h / 2 && posMouse.y <= y + h / 2){
				if (event.type == Event::MouseButtonPressed && !F_pressed){
					shape.setPosition ((float) posMouse.x, (float) y);
					x = (int) posMouse.x; F_pressed = true;
					if (name == "MusicSlider"){ //������ �������� ��������� ������
						volumeMusic = (posMouse.x - leftBorder) / (rightBorder - leftBorder) * 100;
						if (!F_musicOff) changeMusicVolume (volumeMusic); //�������� ��������� ������������� � ��������� �����������, ������ �� ����� �������
						else             changeMusicVolume (0);
					}
					if (name == "SoundSlider"){ //������ �������� ��������� ������
						volumeSound = (posMouse.x - leftBorder) / (rightBorder - leftBorder) * 100;
						if (!F_soundOff) changeSoundVolume (volumeSound);
						else             changeSoundVolume (0);
					}
				}
				if (F_pressed){ //���� ������ ������
					shape.setPosition ((float) posMouse.x, (float) y);
					x = (int) posMouse.x; 
					if (name == "MusicSlider"){ //������ �������� ��������� ������
						volumeMusic = (posMouse.x - leftBorder) / (rightBorder - leftBorder) * 100;
						if (!F_musicOff) changeMusicVolume (volumeMusic); //�������� ��������� ������������� � ��������� �����������, ������ �� ����� �������
						else             changeMusicVolume (0);
					}
					if (name == "SoundSlider"){ //������ �������� ��������� ������
						volumeSound = (posMouse.x - leftBorder) / (rightBorder - leftBorder) * 100;
						if (!F_soundOff) changeSoundVolume (volumeSound);
						else             changeSoundVolume (0);
					}
				}

				if (event.type == Event::MouseButtonPressed) //���� ����� ������, �� ������������� �����. ����
					F_click = true;
				else
					F_click = false;
		}

		if (event.type == Event::MouseButtonReleased) //���� ��������� �����
			F_pressed = false;

		if ((posMouse.x >= x - w / 2) && (posMouse.x <= x + w / 2) && (posMouse.y >= y - h / 2) && (posMouse.y <= y + h / 2)) //���� ������ ���� ��������� �� ������
			shape.setTextureRect (IntRect (wTexture, 4 * hTexture, wTexture, hTexture)); //���� ������� ������ �� �����, �� ������ ������ ��������
		else
			shape.setTextureRect (IntRect (0, 4 * hTexture, wTexture, hTexture));
	}

	void EFF_reduce (){ //���������� ���������� ������, ������ ��� ���� ��� � ������� �����������, � ������ ���
		if (F_transformation){
			shape.setSize (Vector2f ((float) w * reducePrecent / 100, (float) h * reducePrecent / 100));
			shape.setOrigin ((float) w * reducePrecent / 100 / 2, (float) h * reducePrecent / 100 / 2);
			shape.setPosition ((float) x, (float) y);

			backgroundd.setSize (Vector2f ((float) W_BUTTON * reducePrecent / 100, (float) H_BUTTON * reducePrecent / 100));
			backgroundd.setOrigin ((float) W_BUTTON * reducePrecent / 100 / 2, (float) H_BUTTON * reducePrecent / 100 / 2);
			backgroundd.setPosition ((float) x2, (float) y2);

			reducePrecent -= speedChangeSt * time;
			if (reducePrecent < speedChangeSt * time){
				reducePrecent = 100;
				F_transformation = false;
			}
		}
	}

	void EFF_enlarge (){ //���������� ���������� ������, ������ ��� ���� ��� � ������� �����������, � ������ ���
		if (F_transformation){
			shape.setSize (Vector2f ((float) w * enlargePrecent / 100, (float) h * enlargePrecent / 100));
			shape.setOrigin ((float) w * enlargePrecent / 100 / 2, (float) h * enlargePrecent / 100 / 2);
			shape.setPosition ((float) x, (float) y);

			backgroundd.setSize (Vector2f ((float) W_BUTTON * enlargePrecent / 100, (float) H_BUTTON * enlargePrecent / 100));
			backgroundd.setOrigin ((float) W_BUTTON * enlargePrecent / 100 / 2, (float) H_BUTTON * enlargePrecent / 100 / 2);
			backgroundd.setPosition ((float) x2, (float) y2);

			enlargePrecent += speedChangeSt * time;
			if (enlargePrecent > 100 - speedChangeSt * time){
				enlargePrecent = 1;
				F_transformation = false;
			}
		}
	}

	void clear (){
		shape.setSize (Vector2f (1, 1));
		backgroundd.setSize (Vector2f (1, 1));
	}

	void updateText (char *Pass){ }
};

class PictureButton : public BodyButton{ //������ � ��������� (������������ ������ � ����� ����)
private:
	Texture texturePicture; //�������� ��������
	RectangleShape picture; //���� ��������
	int wPicture; //������ ��������
	int hPicture; //������ ��������
	CreateWall typeWall; //�� ������� ����������� ������� ����� ����, � ��� ��� ���������� ������ ��� ����� ������� ����������
public:
	PictureButton (Texture *tmpTexture, String Name, SubtypeState &Subtype, int X, int Y, int W, int H, int WTexture, int HTexture, Image &Ipicture, int WPicture, int HPicture) : 
		    BodyButton (tmpTexture, Name, Subtype, X, Y, W, H, WTexture, HTexture){
		
		shape.setTextureRect (IntRect (0, 120, wTexture, hTexture));
        texturePicture.loadFromImage (Ipicture);
		wPicture = WPicture; hPicture = HPicture;
		picture.setSize (Vector2f ((float) w, (float) h));
		picture.setPosition ((float) x, (float) y);
		picture.setTexture (&texturePicture);
			
		picture.setOrigin ((float) w / 2, (float) h / 2);
		//�� ����� ���������� ��� � ����� �������� �������
		if (name == "Rectangle")       { typeWall = rectangleW; picture.setTextureRect (IntRect (0, 0 * hPicture, wPicture, hPicture)); }
		else if (name == "Triangle")   { typeWall = triangleW;  picture.setTextureRect (IntRect (0, 1 * hPicture, wPicture, hPicture)); }
		else if (name == "Circle")     { typeWall = circleW;    picture.setTextureRect (IntRect (0, 2 * hPicture, wPicture, hPicture)); }
		else if (name == "Wall")       { typeWall = wall;       picture.setTextureRect (IntRect (0, 3 * hPicture, wPicture, hPicture)); }
		else if (name == "Start")      { typeWall = startW;     picture.setTextureRect (IntRect (0, 4 * hPicture, wPicture, hPicture)); }
		else if (name == "Finish")     { typeWall = finishW;    picture.setTextureRect (IntRect (0, 5 * hPicture, wPicture, hPicture)); }
		else if (name == "Save")       { typeWall = saveW;      picture.setTextureRect (IntRect (0, 6 * hPicture, wPicture, hPicture)); }
	}

	void draw (){
		window -> draw (shape);
		window -> draw (picture);
	}

	void checkCursor (){ //������� �������� �� ������� ������ ��� ���������� ������� �� ������
		F_click = false;
		if ((posMouse.x >= x - w / 2) && (posMouse.x <= x + w / 2) && (posMouse.y >= y - h / 2) && (posMouse.y <= y + h / 2)){ //���� ������ ���� ��������� �� ������
			if (Mouse::isButtonPressed (Mouse::Left)) //� ���� ������ �� ���
				F_pressed = true;
			else{
				if (F_pressed){ //���� �� ������ �� ������ � ������ ���� ������, � ������ �� ������-������ �� �������� �� ���
					F_click = true; whichWall = typeWall;
				}
				F_pressed = false;
			}
			shape.setTextureRect (IntRect (wTexture, 4 * hTexture, wTexture, hTexture));
		}
		else{
			F_pressed = false; //���� ������ �� �� ���� �� ������ �������� ����
			shape.setTextureRect (IntRect (0, 4 * hTexture, wTexture, hTexture));
		}		

		if (typeWall == whichWall) //���� ��� ��������� ������ ������������ � �������, �� ��� ������������ � ���������� ����
			shape.setTextureRect (IntRect (2 * wTexture, 4 * hTexture, wTexture, hTexture));
		else
			if (name == "Rectangle")       picture.setTextureRect (IntRect (0, 0 * hPicture, wPicture, hPicture));
			else if (name == "Triangle")   picture.setTextureRect (IntRect (0, 1 * hPicture, wPicture, hPicture));
			else if (name == "Circle")     picture.setTextureRect (IntRect (0, 2 * hPicture, wPicture, hPicture));
			else if (name == "Wall")       picture.setTextureRect (IntRect (0, 3 * hPicture, wPicture, hPicture));
			else if (name == "Start")      picture.setTextureRect (IntRect (0, 4 * hPicture, wPicture, hPicture));
			else if (name == "Finish")     picture.setTextureRect (IntRect (0, 5 * hPicture, wPicture, hPicture));
			else if (name == "Save")       picture.setTextureRect (IntRect (0, 6 * hPicture, wPicture, hPicture));
	}

	void updateText (char *Pass){ }

	void EFF_reduce (){ //���������� ���������� ������, �.�. ���� ��������
		if (F_transformation){
			shape.setSize (Vector2f ((float) w * reducePrecent / 100, (float) h * reducePrecent / 100));
			shape.setOrigin ((float) w * reducePrecent / 100 / 2, (float) h * reducePrecent / 100 / 2);
			shape.setPosition ((float) x, (float) y);

			picture.setSize (Vector2f ((float) w * reducePrecent / 100, (float) h * reducePrecent / 100));
			picture.setOrigin ((float) w * reducePrecent / 100 / 2, (float) h * reducePrecent / 100 / 2);
			picture.setPosition ((float) x, (float) y);

			reducePrecent -= speedChangeSt * time;
			if (reducePrecent < speedChangeSt * time){
				reducePrecent = 100;
				F_transformation = false;
			}
		}
	}

	void EFF_enlarge (){
		if (F_transformation){ //���������� ���������� ������, �.�. ���� ��������
			shape.setSize (Vector2f ((float) w * enlargePrecent / 100, (float) h * enlargePrecent / 100));
			shape.setOrigin ((float) w * enlargePrecent / 100 / 2, (float) h * enlargePrecent / 100 / 2);
			shape.setPosition ((float) x, (float) y);

			picture.setSize (Vector2f ((float) w * enlargePrecent / 100, (float) h * enlargePrecent / 100));
			picture.setOrigin ((float) w * enlargePrecent / 100 / 2, (float) h * enlargePrecent / 100 / 2);
			picture.setPosition ((float) x, (float) y);

			enlargePrecent += speedChangeSt * time;
			if (enlargePrecent > 100 - speedChangeSt * time){
				enlargePrecent = 1;
				F_transformation = false;
			}
		}
	}

	void clear (){
		shape.setSize (Vector2f (1, 1));
		picture.setSize (Vector2f (1, 1));
	}
};

class CheckButton : public BodyButton{ //������ � �������� (������������ ������ � ���������� ���������)
private:
	bool F_switchOff; //����, ������� ���������� ������������� �������� ��
public:
	CheckButton (Texture *tmpTexture, String Name, SubtypeState &Subtype, int X, int Y, int W, int H, int WTexture, int HTexture) : 
		    BodyButton (tmpTexture, Name, Subtype, X, Y, W, H, WTexture, HTexture){
		F_switchOff = false;
		shape.setTextureRect (IntRect (wTexture * 2, 6 * hTexture, wTexture, hTexture)); 
		if (F_musicOff && name == "SwitchMusic")      F_switchOff = true; //������� � ���������� ����, ��� �������������, � ��������� ������ ��������
		else if (F_soundOff && name == "SwitchSound") F_switchOff = true;
	}

	void draw (){
		window -> draw (shape);
	}

	void checkCursor (){ //������� �������� �� ������� ������ ��� ���������� ������� �� ������
		F_click = false;
		if ((posMouse.x >= x - w / 2) && (posMouse.x <= x + w / 2) && (posMouse.y >= y - h / 2) && (posMouse.y <= y + h / 2)){ //���� ������ ���� ��������� �� ������
			if (Mouse::isButtonPressed (Mouse::Left)) //� ���� ������ �� ���
				F_pressed = true;
			else{
				if (F_pressed){ //���� �� ������ �� ������ � ������ ���� ������, � ������ �� ������-������ �� �������� �� ���
					F_click = true; changeBool (F_switchOff); 
					if (F_switchOff){
						if (name == "SwitchMusic")      F_musicOff = true;
						else if (name == "SwitchSound") F_soundOff = true;
					}
					else{
						if (name == "SwitchMusic")      F_musicOff = false;
						else if (name == "SwitchSound") F_soundOff = false;
					}
				}
				F_pressed = false;
			}
			if (!F_switchOff) shape.setTextureRect (IntRect (wTexture * 1, 6 * hTexture, wTexture, hTexture));
			else          shape.setTextureRect (IntRect (wTexture * 3, 6 * hTexture, wTexture, hTexture));
		}
		else{ //���� ������ �� �� ������
			F_pressed = false; //���� ������ �� �� ���� �� ������ �������� ����
			if (!F_switchOff) shape.setTextureRect (IntRect (wTexture * 0, 6 * hTexture, wTexture, hTexture));
			else          shape.setTextureRect (IntRect (wTexture * 2, 6 * hTexture, wTexture, hTexture));
		}
	}

	void EFF_reduce (){ //���������� ����������
		if (F_transformation){
			shape.setSize (Vector2f ((float) w * reducePrecent / 100, (float) h * reducePrecent / 100));
			shape.setOrigin ((float) w * reducePrecent / 100 / 2, (float) h * reducePrecent / 100 / 2);
			shape.setPosition ((float) x, (float) y);

			reducePrecent -= speedChangeSt * time;
			if (reducePrecent < speedChangeSt * time){
				reducePrecent = 100;
				F_transformation = false;
			}
		}
	}

	void EFF_enlarge (){ //���������� ����������
		if (F_transformation){
			shape.setSize (Vector2f ((float) w * enlargePrecent / 100, (float) h * enlargePrecent / 100));
			shape.setOrigin ((float) w * enlargePrecent / 100 / 2, (float) h * enlargePrecent / 100 / 2);
			shape.setPosition ((float) x, (float) y);

			enlargePrecent += speedChangeSt * time;
			if (enlargePrecent > 100 - speedChangeSt * time){
				enlargePrecent = 1;
				F_transformation = false;
			}
		}
	}

	virtual void clear (){ //������� �������� ������, ����� ��� � ������ ������ ����������� (������ ����)
		shape.setSize (Vector2f (1, 1));
	}

	void updateText (char *Pass){ }
};


class Message : public System{ //����� ��������� (������������ ����� ������������ ������ ���� ���������, �.�. ������ ������ ���� ������ ����� ������)
private:
	int sizeText; //������ ������
	bool F_showMessage; //����, ������� ���������� ����� �� ���������
	float messageTimer; //������� �� ������� ����� ������������ ��������� (�� ������ ���������� � ���������� ������)
	float reducePrecent; //������� ����������
	float enlargePrecent; //������� ����������
	int x; //���������� �� �����������
	int y; //���������� �� ���������
	int phase; //���� ��������� (0-����������, 1-�����, 2-����������)
	Font font; //�����
	Color color; //���� ������ ������
	mcText *text; //����� ������� ��������� �� ������
	String messageText; //����� ������� ����� ������������ �� ������
private:
	void EFF_reduce (){ //���������� ���������
		text -> clear ();
		text -> changeSize (sizeText * (int) reducePrecent / 100); //������ ������
		text -> add (messageText, color);
		text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText * reducePrecent / 100 / 3); //����������� ����� �� ������

		reducePrecent -= speedChangeSt * time; //����� ����������� ��������� �����
		if (reducePrecent < speedChangeSt * time){
			reducePrecent = 100; phase = 0;
			F_showMessage = false;
		}
		
	}

	void EFF_enlarge (){ //���������� ���������
		text -> clear ();
		text -> changeSize (sizeText * (int) enlargePrecent / 100); //������ ������
		text -> add (messageText, color);
		text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText * enlargePrecent / 100 / 3); //����������� ����� �� ������

		enlargePrecent += speedChangeSt * time; //����� ����������� ��������� �����
		if (enlargePrecent > 100 - speedChangeSt * time){
			enlargePrecent = 1;
			phase = 1;

			text -> clear (); //����������� ����� �� �� �����, ������� ������� �� 100% �������
			text -> changeSize (sizeText); 
			text -> add (messageText, color);
			text -> setPosition ((float) x - text -> w / 2, (float) y - 2 * sizeText / 3); //����������� ����� �� ������
		}
	}
public:
	void initialize (Font &tmpFont){ //������������� ���������
		font = tmpFont;
		text = new mcText (&font); //������� ����� ������� ����� ������������ �� ������
		text -> changeSize (sizeText); //������ ������
		text -> add ("", color);
		reducePrecent = 100; enlargePrecent = 1; 
		phase = 0; sizeText = SIZE_TEXT;
	}

	void setColor (Color &tmpColor){ //���������� ����
		color = tmpColor;
	}

	void showMessage (int tmpX, int tmpY, String tmpText, Color tmpColor, float tmpTimer){ //����� ��������� � ������
		if (!F_showMessage){ //����� ������ ��������� ��������� ������ ���� ��������� �� ������������
			x = tmpX; y = tmpY;
			F_showMessage = true;
			messageText = tmpText;
			messageTimer = tmpTimer;
			color = tmpColor;
			text -> clear ();
			text = new mcText (&font); //������� ����� ������� ����� ������������ �� ������
			text -> changeSize (sizeText); //������ ������
			text -> add (messageText, color);
			text -> setPosition ((float) x - text -> w / 2, (float) y - text -> h / 2); //����������� ����� �� ������
		}
	}

	void showMessage (int tmpX, int tmpY, String tmpText, float tmpTimer){ //��������� ��� �����
		if (!F_showMessage){ //����� ������ ��������� ��������� ������ ���� ��������� �� ������������
			x = tmpX; y = tmpY;
			F_showMessage = true;
			messageText = tmpText;
			messageTimer = tmpTimer;
			text -> clear ();
			text = new mcText (&font); //������� ����� ������� ����� ������������ �� ������
			text -> changeSize (sizeText); //������ ������
			text -> add (messageText, color);
			text -> setPosition ((float) x - text -> w / 2, (float) y - text -> h / 2); //����������� ����� �� ������
		}
	}

	void draw (){
		if (F_showMessage){
			if (F_changeStates) //���� ��������� ������ �������, �� ��������� ����������� ���������� �� ����, �������� ������� �� ��������
				phase = 2;

			if (phase == 0) //���������� ���������
				EFF_enlarge ();
			else if (phase == 1){ //����� ���������
				messageTimer -= time;
				if (messageTimer <= 0)
				phase = 2;
			}
			else if (phase == 2) //���������� ���������
				EFF_reduce ();
			text -> draw (window);
		}
	}
};


class Game : public System{ //��� �������� � ������������� ���� � ���� ������
public:
	bool F_loadLVL; //����, ������� ���������� �������� �� �������
	bool F_reduceWholeType; //����, ������� ���������� ����������� �� ������ ������� ��������� � ����, � �� ������� (�����, ���-�� �������, ����������)
	bool F_enlargeWholeType; //����, ������� ���������� ������������� �� ������ ������� ��������� � ����, � �� ������� (�����, ���-�� �������, ����������)
	bool F_escapeReleased; //����, ����� 1 ���� ������ ��������� (�� ��� ������, � ����� ���������)
	bool F_enterReleased; //����, ����� 1 ���� Enter ��������� (�� ��� ������, � ����� ���������)
	bool F_anyKeyReleased; //����, ����� 1 ���� Enter ��������� (�� ��� ������, � ����� ���������)
	bool F_isPlayerLVL; //����, ������� ���������� ����� ������ � ���� ��������� ������� ��� ���
	bool F_changeKey; //����, ������� ����������, �������� �� ������ ����� ������� (����� ������ ������� �� ������� �������� ������)
	bool F_secPhaseChangeState; //����, ������� ����������, ������ �� ������ ���� ��������� ��������� (����������)
	bool F_inSetingIntoPause; //����, ������� ����������, ����� �� �� � ��������� ����� ������ (����� ��� � ����� �������� �� �������� ������������ � ����)

	char playerLVLOpenByPlayer [50];  //��� ����� ��������� �������, � ������� ��� ���� ��������� �������
	char lvlOpenByAdmin [50]; //��� ����� ��������� �������
	char lvlOpenByPlayer [70]; //��� ����� ��������� �������

	float lvlTime; //����� ����������� ������� �� ������
	float AllTime; //����� ����� ������ ������������ � ����

	int indexFinish; //������ ������ (��� � ����� �� ������)
	int indexStart; //������ ������ (��� � ����� �� ������)
	int indexDeathPlayerBut; //������ ������ �� ������� ��������� ���������� ������� �� ������
	int indexFPSBut; //������ ������ �� ������� ��������� �������� ���
	int indexTimePlBut; //������ ������ �� ������� ��������� ����� ������
	int indexControlBut [4]; //������� ������, �� ������� ���������� ����������
	int CurrentLVL; //������� �������
	int NumWall; //���������� ����
	int NumButton; //���������� ������
	int NumListLVL; //���������� ������� � ������ (��� � ����� ����� ������� ������� ��������� ������)
	int NumLVLDeath; //����������� ������� �� ������

	TypeState type; //������� ���
	SubtypeState subtype; //������� ������
	SubtypeState whichStateWas; //����� ��������� ����, ����� ��� ��������� ���������
	SubtypeState whichStateWill; //����� ��������� ������ �����, ����� ��� ��������� ���������

	Texture *wallImage; //����������� ����
	Texture *wallImagePL; //����������� ������
	Background *plBackground; //������� �����������, ������� ��������� ��������
	Background *logo; //�������
	Background *SFML; //���� SFML
	VertexArray lines; //����� ������� � ����� ���� ��������, ��� � ����� ���� ��������� ������
	RectangleShape cursor; //������
	Texture textureCursor; //�������� ������� 
	Message message; //���������
	Player *pl; //�����
	BodyButton *button [120]; //������ ������
	Wall *ArrWall [2500]; //������ ����
	bool **wallCoordinate; //���������� ����
public:
	void readInfo (){ //������� ��������� ����
		ifstream inF ("Resources/Settings/Settings.prof");
		inF >> passedLVL >> volumeMusic >> volumeSound >> AllTime >> key [0] >> key [1] >> key [2] >> F_musicOff >> F_soundOff; //��������� 3-������ ������ �� ������� �������� ������ ������
		passedLVL -= 139;

		bool tmpB = false;
		if (F_musicOff != 0 && F_musicOff != 1)        tmpB = true;
		else if (F_soundOff != 0 && F_soundOff != 1)   tmpB = true;
		else if (volumeMusic > 100 || volumeMusic < 0) tmpB = true;
		else if (volumeSound > 100 || volumeSound < 0) tmpB = true;
		else if (passedLVL % 23 != 0 || passedLVL < 0) tmpB = true; 

		if (tmpB){
			F_gameIsLocked = true; //���� ���� �� ������� �� ��������� �� ���� �����������, � ����� �� �������� � ����
			cout << "Don't change the settings" << endl;
			message.showMessage (W_WIN / 2, H_WIN / 2 + 3 * H_BUTTON, "Don't change the settings", 50.1f);
			passedLVL += 139;
		}
		else
			passedLVL /= 23;
	}

	void writeInfo (){ //�������� ��������� ����
		ofstream outF ("Resources/Settings/Settings.prof");
		outF << passedLVL * 23 + 139 << " " << volumeMusic << " " << volumeSound  << " " << AllTime;
		outF << " " << key [0] << " " << key [1] << " " << key [2] << " " << F_musicOff << " " << F_soundOff << endl; //��������� 3-������ ������ �� ������� �������� ������ ������
		outF << "Close this document immediately, else this will lead to problems." << endl;
	}

	void draw (){ //������� � ������������ ������� ���������
		window -> setView (view); //��������� ������
		window -> clear (Color (117, 152, 175)); //�������� ������ ���

		if (type == admin && subtype != listLVL){ //� ����� ����
			window -> draw (lines); //����� ��������������� ����� ��� ������
			for (int i = 0; i < NumWall; i++) //����� ����� ������
				ArrWall [i] -> draw ();
		}

		if (type == player){ //��, ��� ����� �����
			window -> draw (map); //����� ����� ������� ��� ���� ������
			plBackground -> draw (); //����� ���, ������� ��������� ������� ���� ������� ���������
			pl -> draw (); //����� ������
			button [indexTimePlBut] -> draw (); //����� ������ ��� ������������ ����� (�� �������� ���������� ��� ������ �������)
			button [indexDeathPlayerBut] -> draw (); //����� ������ ��� ������������ ���������� ������� �� ������
			for (int i = 0; i < 4; i++) //����� ������ �� ������� �������� ����������
				button [indexControlBut [i]] -> draw ();
		}
		else if (type == menu || subtype == loadingLVL){ //����� ������� � ������ SFML
			logo -> draw (); SFML -> draw ();
		}

		for (int i = 0; i < NumButton; i++) //����� ������
			if (button [i] -> F_draw)
				button [i] -> draw ();
		button [indexFPSBut] -> draw (); //������ ��� ������������ ��� ����� ������

		cursor.setPosition ((float) posMouse.x, (float) posMouse.y); //������������ ������ ������� � �� ����� ���� ��������� ������ ����
		window -> draw (cursor); //����� ������ �������

		message.draw (); //����� ��������� (��� �� �������� ���� ��������� �� ���� ������)

		window -> display (); //��������� ��� ��� ������������
	}

	void initializeBackground (){ //������������� ������� ����
		Texture *backgroundTex = new Texture;
		backgroundTex -> loadFromFile ("Resources/Textures/background.png"); //��� ������� ��������� ������� ���� ����� ������ �����
		plBackground = new Background (backgroundTex, "PlayerBackground", 0, 0, 3000 * (NUM_SQUARE) * SQUARE / 500, 3000 * (NUM_SQUARE) * SQUARE / 500, 3000, 3000); //�� ����� ����� ��������� 2 ���������
		plBackground -> changeLocation (W_WIN / 2, H_WIN / 2);

		Texture *backgroundTex2 = new Texture; //�������� ����
		backgroundTex2 -> loadFromFile ("Resources/Textures/logo2.png"); //�������
		logo = new Background (backgroundTex2, "Logo", 0, 0, W_BUTTON * 3, H_BUTTON * 3, 392, 91); 
		logo -> changeLocation (W_WIN / 2, H_WIN / 2 - 7 * (H_BUTTON + INTERVAL)); 

		Texture *backgroundTex3 = new Texture;
		backgroundTex3 -> loadFromFile ("Resources/Textures/SFML.png"); //������ SFML
		SFML = new Background (backgroundTex3, "SFML", 0, 0, H_BUTTON * 2, H_BUTTON * 2, 256, 256); 
		SFML -> changeLocation (W_WIN / 2 + W_WIN / 2 - H_BUTTON * 2, H_WIN / 2 + H_WIN / 2 - H_BUTTON * 2); 
	}

	void initializeButtons (){
		Font font;
		font.loadFromFile ("Resources/Fonts/modeka.otf");

		Texture *buttonTex = new Texture; //�������� �������� ����
		buttonTex -> loadFromFile ("Resources/Textures/button.png");

		SubtypeState tmpS;
		int tmpI;

		NumButton = 0;

		tmpS = wholeType;
		indexControlBut [0] = NumButton + 0;
		indexControlBut [1] = NumButton + 1;
		indexControlBut [2] = NumButton + 2;
		indexControlBut [3] = NumButton + 3;
		indexTimePlBut = NumButton + 4;
		indexDeathPlayerBut = NumButton + 5;
		tmpI = NUM_SQUARE * SQUARE / 2 + (W_WIN - NUM_SQUARE * SQUARE) / 4;
		button [NumButton++] = new Static ("Rectangle: 1", "ControlRec",  font, tmpS, W_WIN / 2 - tmpI, H_WIN / 2 + 2 * (H_BUTTON + INTERVAL), Color (25, 36, 68));
		button [NumButton++] = new Static ("Triangle: 2",  "ControlTri",  font, tmpS, W_WIN / 2 - tmpI, H_WIN / 2 + 3 * (H_BUTTON + INTERVAL), Color (25, 36, 68));
		button [NumButton++] = new Static ("Circle: 3",    "ControlCir",  font, tmpS, W_WIN / 2 - tmpI, H_WIN / 2 + 4 * (H_BUTTON + INTERVAL), Color (25, 36, 68));
		button [NumButton++] = new Static ("Control",      "Control",     font, tmpS, W_WIN / 2 - tmpI, H_WIN / 2 + 1 * (H_BUTTON + INTERVAL), Color (6, 10, 25));
		button [NumButton++] = new Static ("Time: 0",      "TimePlayer",  font, tmpS, W_WIN / 2 - tmpI, H_WIN / 2 - NUM_SQUARE * SQUARE / 2 + (H_BUTTON + INTERVAL), Color (211, 25, 12));
		button [NumButton++] = new Static ("Death: 0",     "DeathPlayer", font, tmpS, W_WIN / 2 - tmpI, H_WIN / 2 - NUM_SQUARE * SQUARE / 2 + 2 * (H_BUTTON + INTERVAL), Color (211, 25, 12));

		tmpS = loadingLVL;
		button [NumButton++] = new Static ("Loading...", "LoadingLVL", font, tmpS, W_WIN / 2, H_WIN / 2, Color (6, 10, 25));

		tmpS = allState;
		indexFPSBut = NumButton;
		button [NumButton++] = new Static ("FPS: 0", "FPS", font, tmpS, W_BUTTON / 3, H_WIN - H_BUTTON / 3, 2 * SIZE_TEXT / 3);


		tmpS = launcher;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Button (buttonTex, "Go",      "Go!",      font, tmpS, W_WIN / 2, H_WIN / 2 - 3 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Mode",    "Mode",     font, tmpS, W_WIN / 2, H_WIN / 2 - 2 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Setting", "Settings", font, tmpS, W_WIN / 2, H_WIN / 2 - 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "About",   "About",    font, tmpS, W_WIN / 2, H_WIN / 2 - 0 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Exit",    "Exit",     font, tmpS, W_WIN / 2, H_WIN / 2 + 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = exitt;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Static (             "Quit game?", "Quit game?", font, tmpS, W_WIN / 2, H_WIN / 2 - 0 * tmpI);
		button [NumButton++] = new Button (buttonTex, "No!",        "QuitNo",     font, tmpS, W_WIN / 2, H_WIN / 2 + 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Yes.",       "QuitYes",    font, tmpS, W_WIN / 2, H_WIN / 2 + 2 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = settings;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Button (buttonTex, "Controls", "ControlsSet",   font, tmpS, W_WIN / 2, H_WIN / 2 - 2 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Audio",    "AudioSet",      font, tmpS, W_WIN / 2, H_WIN / 2 - 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Back",     "BackToMenuSet", font, tmpS, W_WIN / 2, H_WIN / 2 - 0 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = controlSeting;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Static (                 "Rectangle:", "Rectangle",        font, tmpS, W_WIN / 2 - W_BUTTON / 2, H_WIN / 2 - 3 * tmpI);
		button [NumButton++] = new Static (                 "Triangle:",  "Triangle",         font, tmpS, W_WIN / 2 - W_BUTTON / 2, H_WIN / 2 - 2 * tmpI);
		button [NumButton++] = new Static (                 "Circle:",    "Circle",           font, tmpS, W_WIN / 2 - W_BUTTON / 2, H_WIN / 2 - 1 * tmpI);
		button [NumButton++] = new EditButton (buttonTex, "1",          "ChangeKey",        font, tmpS, W_WIN / 2 + W_BUTTON / 4, H_WIN / 2 - 3 * tmpI, W_BUTTON / 4, H_BUTTON, 47,  45);
		button [NumButton++] = new EditButton (buttonTex, "2",          "ChangeKey",        font, tmpS, W_WIN / 2 + W_BUTTON / 4, H_WIN / 2 - 2 * tmpI, W_BUTTON / 4, H_BUTTON, 47,  45);
		button [NumButton++] = new EditButton (buttonTex, "3",          "ChangeKey",        font, tmpS, W_WIN / 2 + W_BUTTON / 4, H_WIN / 2 - 1 * tmpI, W_BUTTON / 4, H_BUTTON, 47,  45);
		button [NumButton++] = new Button (buttonTex,     "Back",       "BackToControlSet", font, tmpS, W_WIN / 2,                H_WIN / 2 - 0 * tmpI, W_BUTTON,     H_BUTTON, 0, 188, 45);
		
		tmpS = audioSetting;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Static (             "Sound:",  "VolSound",       font, tmpS, W_WIN / 2 - W_BUTTON / 2 - tmpI, H_WIN / 2 - 2 * tmpI);
		button [NumButton++] = new Static (             "Music:",  "VolMusic",       font, tmpS, W_WIN / 2 - W_BUTTON / 2 - tmpI, H_WIN / 2 - 1 * tmpI);
		button [NumButton++] = new Button (buttonTex, "Back",    "BackToSetAudio", font, tmpS, W_WIN / 2,                       H_WIN / 2 - 0 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		button [NumButton++] = new CheckButton (buttonTex,       "SwitchSound", tmpS, W_WIN / 2 + W_BUTTON / 2 + 3 * tmpI / 2, H_WIN / 2 - 2 * tmpI,     H_BUTTON, H_BUTTON, 47, 45);
		button [NumButton++] = new CheckButton (buttonTex,       "SwitchMusic", tmpS, W_WIN / 2 + W_BUTTON / 2 + 3 * tmpI / 2, H_WIN / 2 - 1 * tmpI,     H_BUTTON, H_BUTTON, 47, 45);
		button [NumButton++] = new HorizontScrollBar (buttonTex, "SoundSlider", tmpS, W_WIN / 2 + W_BUTTON / 2 - tmpI,         H_WIN / 2 - 2 * tmpI, 20, H_BUTTON - 4, W_WIN / 2 - tmpI + 13, W_WIN / 2 + W_BUTTON - tmpI - 13, 47, 45, 188, 45);
		button [NumButton++] = new HorizontScrollBar (buttonTex, "MusicSlider", tmpS, W_WIN / 2 + W_BUTTON / 2 - tmpI,         H_WIN / 2 - 1 * tmpI, 20, H_BUTTON - 4, W_WIN / 2 - tmpI + 13, W_WIN / 2 + W_BUTTON - tmpI - 13, 47, 45, 188, 45);
		
		tmpS = mode;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Button (buttonTex, "Player", "PlayerMode", font, tmpS, W_WIN / 2, H_WIN / 2 - 3 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Admin",  "AdminMode",  font, tmpS, W_WIN / 2, H_WIN / 2 - 2 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Back",   "BackToMenu", font, tmpS, W_WIN / 2, H_WIN / 2 - 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = selectLVL;
		/*tmpI = H_WIN / 2 - 3 * (H_BUTTON + INTERVAL);
		button [NumButton++] = new Button (buttonTex, "1",          "SelectLVL",     font, tmpS, W_WIN / 2 - 3 * (W_BUTTON) / 8, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 1, 47,  45);
		button [NumButton++] = new Button (buttonTex, "2",          "SelectLVL",     font, tmpS, W_WIN / 2 - (W_BUTTON) / 8 - 1, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 2, 47,  45);
		button [NumButton++] = new Button (buttonTex, "3",          "SelectLVL",     font, tmpS, W_WIN / 2 + (W_BUTTON) / 8 + 1, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 3, 47,  45);
		button [NumButton++] = new Button (buttonTex, "4",          "SelectLVL",     font, tmpS, W_WIN / 2 + 3 * (W_BUTTON) / 8, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 4, 47,  45);*/
		tmpI = H_WIN / 2 - 2 * (H_BUTTON + INTERVAL);
		button [NumButton++] = new Button (buttonTex, "1",          "SelectLVL",     font, tmpS, W_WIN / 2 - 3 * (W_BUTTON) / 8, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 1, 47,  45);
		button [NumButton++] = new Button (buttonTex, "2",          "SelectLVL",     font, tmpS, W_WIN / 2 - (W_BUTTON) / 8 - 1, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 2, 47,  45);
		button [NumButton++] = new Button (buttonTex, "3",          "SelectLVL",     font, tmpS, W_WIN / 2 + (W_BUTTON) / 8 + 1, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 3, 47,  45);
		button [NumButton++] = new Button (buttonTex, "4",          "SelectLVL",     font, tmpS, W_WIN / 2 + 3 * (W_BUTTON) / 8, tmpI,                (W_BUTTON - 4) / 4, H_BUTTON, 4, 47,  45);
		button [NumButton++] = new Button (buttonTex, "My lvls",    "My lvls",       font, tmpS, W_WIN / 2, H_WIN / 2 - 1 * (H_BUTTON + INTERVAL), W_BUTTON,           H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Back",       "BackToMenuSel", font, tmpS, W_WIN / 2, H_WIN / 2 - 0 * (H_BUTTON + INTERVAL), W_BUTTON,           H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Static (             "Select LVL", "SelectStatic",  font, tmpS, W_WIN / 2, H_WIN / 2 - 3 * (H_BUTTON + INTERVAL));

		tmpS = playerLVL;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new EditButton (buttonTex, "",                    "InputMyLVL",      font, tmpS, W_WIN / 2, H_WIN / 2 - 2 * tmpI, W_BUTTON, H_BUTTON,    188, 45);
		button [NumButton++] = new Static (                 "Enter name your LVL", "StaticMyLVL",     font, tmpS, W_WIN / 2, H_WIN / 2 - 3 * tmpI);
		button [NumButton++] = new Button (buttonTex,     "Back",                "BackToMenuMyLVL", font, tmpS, W_WIN / 2, H_WIN / 2 - 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = about;
		tmpI = H_BUTTON + INTERVAL;
		button [NumButton++] = new Static ("Developer:  tiki",           "Developer", font, tmpS, W_WIN / 2, H_WIN / 2 - 3 * tmpI);
		button [NumButton++] = new Static ("Version:     1.0",             "Version", font, tmpS, W_WIN / 2, H_WIN / 2 - 2 * tmpI);
		button [NumButton++] = new Static ("I want to thank: Blizzru, With_Drollery", "Thnaks1",  font, tmpS, W_WIN / 2, H_WIN / 2 - 1 * tmpI);
		button [NumButton++] = new Static ("and my brother.",              "Thanks3", font, tmpS, W_WIN / 2, H_WIN / 2 - 0 * tmpI);
		button [NumButton++] = new Button (buttonTex, "Back",    "BackToMenuAbout", font, tmpS, W_WIN / 2, H_WIN / 2 + 1 * tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);


		Image pictureImage; //�������� ������� ����
		tmpS = editLVL;
		tmpI = H_WIN / 2 + (H_WIN + NUM_CELL_Y * EDGE) / 4;
		button [NumButton++] = new Button (buttonTex, "Back",     "BackToMenuAd", font, tmpS, W_WIN / 2 - 2 * W_BUTTON - 6 * EDGE, tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Open",     "OpenAd",       font, tmpS, W_WIN / 2 - 1 * W_BUTTON - 3 * EDGE, tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Save",     "SaveAd",       font, tmpS, W_WIN / 2 + 0 * W_BUTTON + 0 * EDGE, tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Delete",   "DeleteAd",     font, tmpS, W_WIN / 2 + 1 * W_BUTTON + 3 * EDGE, tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "List lvl", "ListAd",       font, tmpS, W_WIN / 2 + 2 * W_BUTTON + 6 * EDGE, tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);
		pictureImage.loadFromFile ("Resources/Textures/button2.png"); 
		tmpI = H_WIN / 2 - (H_WIN + NUM_CELL_Y * EDGE) / 4;
		button [NumButton++] = new PictureButton (buttonTex, "Rectangle", tmpS, W_WIN / 2 - 9 - H_BUTTON * 3, tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30);
		button [NumButton++] = new PictureButton (buttonTex, "Triangle",  tmpS, W_WIN / 2 - 6 - H_BUTTON * 2, tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30);
		button [NumButton++] = new PictureButton (buttonTex, "Circle",    tmpS, W_WIN / 2 - 3 - H_BUTTON,     tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30);
		button [NumButton++] = new PictureButton (buttonTex, "Wall",      tmpS, W_WIN / 2,                    tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30);
		button [NumButton++] = new PictureButton (buttonTex, "Start",     tmpS, W_WIN / 2 + 3 + H_BUTTON,     tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30); 
		button [NumButton++] = new PictureButton (buttonTex, "Finish",    tmpS, W_WIN / 2 + 6 + H_BUTTON * 2, tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30);
		button [NumButton++] = new PictureButton (buttonTex, "Save",      tmpS, W_WIN / 2 + 9 + H_BUTTON * 3, tmpI, H_BUTTON, H_BUTTON, 47, 45, pictureImage, 30, 30);

		tmpS = openLVL;
		tmpI = (H_WIN + NUM_CELL_Y * EDGE) / 4;
		button [NumButton++] = new EditButton (buttonTex, "", "EditLVL",        font, tmpS, W_WIN / 2, H_WIN / 2 - tmpI, W_BUTTON, H_BUTTON,    188, 45);
		button [NumButton++] = new Button (buttonTex, "Back", "BackToAdminSel", font, tmpS, W_WIN / 2, H_WIN / 2 + tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = saveLVL;
		tmpI = (H_WIN + NUM_CELL_Y * EDGE) / 4;
		button [NumButton++] = new EditButton (buttonTex, "", "AdSaveLVL",       font, tmpS, W_WIN / 2, H_WIN / 2 - tmpI, W_BUTTON, H_BUTTON,    188, 45);
		button [NumButton++] = new Button (buttonTex, "Back", "BackToAdminSave", font, tmpS, W_WIN / 2, H_WIN / 2 + tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = deleteLVL;
		tmpI = (H_WIN + NUM_CELL_Y * EDGE) / 4;
		button [NumButton++] = new EditButton (buttonTex, "", "AdDeleteLVL",       font, tmpS, W_WIN / 2, H_WIN / 2 - tmpI, W_BUTTON, H_BUTTON,    188, 45);
		button [NumButton++] = new Button (buttonTex, "Back", "BackToAdminDelete", font, tmpS, W_WIN / 2, H_WIN / 2 + tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = listLVL;
		tmpI = (H_WIN + NUM_CELL_Y * EDGE) / 4;
		button [NumButton++] = new Button (buttonTex, "Back", "BackToAdminList", font, tmpS, W_WIN / 2, H_WIN / 2 + tmpI, W_BUTTON, H_BUTTON, 0, 188, 45);


		tmpS = play;
		tmpI = NUM_SQUARE * SQUARE / 2 + (W_WIN - NUM_SQUARE * SQUARE) / 4;
		button [NumButton++] = new Button (buttonTex,    "Pause",       "BackToMenuPl", font, tmpS, W_WIN / 2 + tmpI, H_WIN / 2 - 0 * (H_BUTTON + INTERVAL), W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = pause;
		tmpI = W_WIN / 2 + NUM_SQUARE * SQUARE / 2 + (W_WIN - NUM_SQUARE * SQUARE) / 4;
		button [NumButton++] = new Static (             "PAUSE",    "Pause",             font, tmpS, tmpI, H_WIN / 2 - 2 * (H_BUTTON + INTERVAL), Color (25, 36, 68));
		button [NumButton++] = new Button (buttonTex, "Play",     "BackToPlPause",     font, tmpS, tmpI, H_WIN / 2 - 1 * (H_BUTTON + INTERVAL), W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Setting",  "SettingsIntoPause", font, tmpS, tmpI, H_WIN / 2 - 0 * (H_BUTTON + INTERVAL), W_BUTTON, H_BUTTON, 0, 188, 45);
		button [NumButton++] = new Button (buttonTex, "Exit",     "LeaveToSel",        font, tmpS, tmpI, H_WIN / 2 + 1 * (H_BUTTON + INTERVAL), W_BUTTON, H_BUTTON, 0, 188, 45);

		tmpS = startLVL;
		tmpI = W_WIN / 2 + NUM_SQUARE * SQUARE / 2 + (W_WIN - NUM_SQUARE * SQUARE) / 4;
		button [NumButton++] = new Static ("Press",            "StartLVL", font, tmpS, tmpI, H_WIN / 2 - 2 * (H_BUTTON + INTERVAL), Color (6, 10, 25));
		button [NumButton++] = new Static ("Escape to leave",  "StartLVL", font, tmpS, tmpI, H_WIN / 2 - 1 * (H_BUTTON + INTERVAL), Color (6, 10, 25));
		button [NumButton++] = new Static ("Any key to start", "StartLVL", font, tmpS, tmpI, H_WIN / 2 - 0 * (H_BUTTON + INTERVAL), Color (6, 10, 25));
	}

	void initializeLines (){
		int GLOB_IND_W = (W_WIN - NUM_CELL_X * EDGE) / 2; //������ �� ������, � �������� ���������� �������
		int GLOB_IND_H = (H_WIN - NUM_CELL_Y * EDGE) / 2; //������ �� ������, � �������� ���������� �������

		lines = VertexArray (Lines, (NUM_CELL_Y + NUM_CELL_X + 4) * 2); //������ �����
		int i = 0; //i-������� ����� ���������� � ������
		for (int k = 0; k < (NUM_CELL_Y + NUM_CELL_X + 2) * 2; k++)
			lines [k].color = Color (43, 34, 65); //30, 30, 30//79,75,5
		for (; i < (NUM_CELL_X + 1) * 2; i += 2){ //�������� ������������ �����
			lines [i].position = Vector2f ((float) GLOB_IND_W + i * EDGE / 2, (float) GLOB_IND_H);
			lines [i + 1].position = Vector2f ((float) GLOB_IND_W + i * EDGE / 2, (float) GLOB_IND_H + NUM_CELL_Y * EDGE);
		}
		for (int k = 0; i < (NUM_CELL_Y + NUM_CELL_X + 2) * 2; i += 2, k += 2){ //�������� �������������� �����
			lines [i].position = Vector2f ((float) GLOB_IND_W, (float) EDGE * k / 2 + GLOB_IND_H);
			lines [i + 1].position = Vector2f ((float) GLOB_IND_W + NUM_CELL_X * EDGE, (float) EDGE * k / 2 + GLOB_IND_H);
		}
	}

	void initializeSomething (){
		Start.x = 0; Start.y = 0; //�������������� ��������� �����
		Finish.x = 0; Finish.y = 0; //�������������� �����

		Texture *playerTex = new Texture; //������� ������� ������
		playerTex -> loadFromFile ("Resources/Textures/player2.png");
		pl  = new Player (playerTex, Start.x, Start.y, SQUARE, SQUARE, 40, 40); //�������� ������� �����

		backgroundMusic.openFromFile ("Resources/Music/Background.ogg"); //������
		backgroundMusic.play (); 
		backgroundMusic.setLoop (true);
		if (!F_musicOff) changeMusicVolume (volumeMusic);
		else             changeMusicVolume (0);

		bufferClickButt.loadFromFile ("Resources/Sounds/button_click.wav"); //���� ������� �� ������
		sndClickButt.setBuffer (bufferClickButt);
		bufferTeleport.loadFromFile ("Resources/Sounds/teleportation.wav"); //���� ������������ ������
		sndTeleport.setBuffer (bufferTeleport);
		if (F_soundOff) changeSoundVolume (0);
		else            changeSoundVolume (volumeSound);

		textureCursor.loadFromFile ("Resources/Textures/cursor2.png"); //������
		cursor.setTexture (&textureCursor);
		cursor.setTextureRect (IntRect (0, 0, 46, 54));
		cursor.setSize (Vector2f ((float) 23 * (3 * H_BUTTON / 5) / 27, (float) 3 * H_BUTTON / 5));

		char tmpC [2]; //������� � �������� ����, �� ����� ������ �������� ������, � ��������� ����� ���� ������
		for (int i = 0; i < 3; i++){
			if (key [i] >= 0 && key [i] <= 25){
				tmpC [0] = key [i] + 65;
				tmpC [1] = '\0';
			}
			else if (key [i] >= 26 && key [i] <= 35){
				tmpC [0] = key [i] + 22;
				tmpC [1] = '\0';
			}
			button [indexControlBut [i]] -> updateText (tmpC);
		}

		levelArray = new int [(NUM_CELL_X + 2) * (NUM_CELL_Y + 2)]; //������� ������ �� ������ � ������� �������� �������
	}

	void initializeWalls (){
		wallImage = new Texture;
		wallImagePL = new Texture;
		wallImage -> loadFromFile ("Resources/Textures/wall.png");
		wallImagePL -> loadFromFile ("Resources/Textures/wall2.png");
	
		NumWall = 0; //���������� ����
		wallCoordinate = new bool* [NUM_CELL_X];
		for (int i = 0; i < NUM_CELL_X; i++){
			wallCoordinate [i] = new bool [NUM_CELL_Y];
			for (int j = 0; j < NUM_CELL_Y; j++)
				wallCoordinate [i][j] = false; //����� true ���� ��� ���� �����
		}
	}

	Game (){ //����������� � ������� �������������� �������� ���������
		AdOrPlMode = "PlayerMode"; //������ �������� ��� �������� ���� ���� (����� ��� �����)
		strcpy (lvlOpenByAdmin, "");
		strcpy (playerLVLOpenByPlayer, "");
		F_lvlComplete = false;
		F_isPlayerLVL = false;
		F_escapeReleased = false;
		F_changeStates = false; 
		F_secPhaseChangeState = false;
		F_inSetingIntoPause = false;
		F_changeKey = false;
		F_enlargeWholeType = false;
		F_reduceWholeType = false;
		F_loadLVL = false;
		F_enterReleased = false;
		F_gameIsLocked = false;
		type = menu;
		subtype = launcher;
		whichWall = wall;
		key [0] = 27;
		key [1] = 28;
		key [2] = 29;
		lvlTime = 0;
		CurrentLVL = 1;
		timer = 0;
		NumWall = 0;
		NumLVLDeath = 0;
		NumListLVL = 0;
		whatButChange = 0;
		indexFinish = -1;

		Font font; //������������� ��������� (����� � �� � ������ �����, ��� � ���� ���� ����� �������������, ��� � ����� ���� �������� ���������
		font.loadFromFile ("Resources/Fonts/modeka.otf");
		message.initialize (font);
		message.setColor (Color (75, 30, 34));

		readInfo ();
		initializeButtons (); //�������� ��������� �������������
		initializeSomething ();
		initializeLines ();
		initializeWalls ();
		initializeBackground ();
	}


	void createWalls (){ //�������� ���� � ����� ����
		int tmpX, tmpY;
		int tmp; 
		bool deleted = false;
		bool wallDeleted = false;
		bool circleDeleted = false;
		bool rectangleDeleted = false;
		bool triangleDeleted = false;
		bool saveDeleted = false;
		int GLOB_IND_W = (W_WIN - NUM_CELL_X * EDGE) / 2; //������ �� ������, � �������� ���������� �������
		int GLOB_IND_H = (H_WIN - NUM_CELL_Y * EDGE) / 2; //������ �� ������, � �������� ���������� ������
		if (Mouse::isButtonPressed (Mouse::Left))
			if ((posMouse.x >= GLOB_IND_W) && (posMouse.x <= GLOB_IND_W + NUM_CELL_X * EDGE) && (posMouse.y >= GLOB_IND_H) && (posMouse.y <= GLOB_IND_H + NUM_CELL_Y * EDGE))
				if (timer > 0.3){ //����� ����� ������� ��� � 0.3 ���
					timer = 0;	
					tmpX = (int) posMouse.x; tmpX -= GLOB_IND_W; tmp = tmpX % EDGE; tmpX -= tmp; tmpX /= EDGE; //�������� ���������� ���� � �� 0 �� 31 � �� 0 �� 63
					tmpY = (int) posMouse.y; tmpY -= GLOB_IND_H; tmp = tmpY % EDGE; tmpY -= tmp; tmpY /= EDGE;
					for (int i = 0; i < NumWall; i++){
						if (ArrWall [i] -> x == tmpX && ArrWall [i] -> y == tmpY)
							if (ArrWall [i] -> name != "Start" && ArrWall [i] -> name != "Finish"){ //������ ����� ���� ��� �� ����� ��� �����
								deleted = true;
								if (ArrWall [i] -> name == "Wall")             wallDeleted = true; //��������� ����� ������ ����� � ������ (����������� � ����������)
								else if (ArrWall [i] -> name == "Circle")      circleDeleted = true;
								else if (ArrWall [i] -> name == "Rectangle")   rectangleDeleted = true;
								else if (ArrWall [i] -> name == "Triangle")    triangleDeleted = true;
								else if (ArrWall [i] -> name == "Save")        saveDeleted = true;
								if (i != NumWall - 1) //����� ��������� ����� �� �� ������� ���� �������
									*ArrWall [i] = *ArrWall [NumWall - 1];
								if (ArrWall [NumWall - 1] -> name == "Start") //����� ������ ������ � ������ (���� ����� ��������� ������ �������� ����� ��� �����
									indexStart = i;
								else if (ArrWall [NumWall - 1] -> name == "Finish")
									indexFinish = i;
								wallCoordinate [tmpX][tmpY] = false;
								delete ArrWall [NumWall - 1]; //������ ��������� ����� (�� ���� ����������� ����� ������� ���� �������)
								NumWall--; //��������� ���������� ����
								break;
							}
					}

					if (whichWall == startW){ //���� �� ������ �����
						if (tmpX != ArrWall [indexFinish] -> x || tmpY != ArrWall [indexFinish] -> y){ //���� �� ����� ��������� ����� �� �� ����� (�� ����� ������ ���������)
							*ArrWall [indexStart] = *ArrWall [NumWall - 1]; //������� ������ ����� �� ��������� �����

							if (ArrWall [NumWall - 1] -> name == "Finish") //���� ����� �� ��������� ����� ��� �����
								indexFinish = indexStart;

							delete ArrWall [NumWall - 1]; //������ ������ �����
							wallCoordinate [tmpX][tmpY] = false;
							NumWall--;

							indexStart = NumWall; //����� ������ � ������ �����
							ArrWall [NumWall++] = new Wall (wallImage, "Start", tmpX, tmpY, EDGE, EDGE, 20, 20);
							Start.x = tmpX; Start.y = tmpY;
						}
					}
					else if (whichWall == finishW){ //���������� ��� �� ������� ������ ��� ������
						if (tmpX != ArrWall [indexStart] -> x || tmpY != ArrWall [indexStart] -> y){
							*ArrWall [indexFinish] = *ArrWall [NumWall - 1];

							if (ArrWall [NumWall - 1] -> name == "Start")
								indexStart = indexFinish;

							delete ArrWall [NumWall - 1];
							wallCoordinate [tmpX][tmpY] = false;
							NumWall--;

							indexFinish = NumWall;
							ArrWall [NumWall++] = new Wall (wallImage, "Finish", tmpX, tmpY, EDGE, EDGE, 20, 20);
							Finish.x = tmpX; Finish.y = tmpY;
						}
					}
					else{ //���� �� ����� ��������� �� �� ����� � ����� � ����� ��������� ���-�� ���� ������ � ������
						if ((tmpX == ArrWall [indexStart] -> x && tmpY == ArrWall [indexStart] -> y) || (tmpX == ArrWall [indexFinish] -> x && tmpY == ArrWall [indexFinish] -> y))
							tmp = 0;
						else { //���� �� �������� ����������� ��������� �� �������, �� ������� ���������, � ������ ���� �������� �����������, ���� ����������� �� ����������� ������, �� ����������� ������ ���������
							//��� ����� � ����� ���� ���������� ����� ����� �� ������� �������
							if ((whichWall == wall) && !wallDeleted){
								ArrWall [NumWall++] = new Wall (wallImage, "Wall", tmpX, tmpY, EDGE, EDGE, 20, 20);
								wallCoordinate [tmpX][tmpY] = true; //���� ��� �����, �� ����� �������� ������ ����
							}
							else if ((whichWall == rectangleW) && !rectangleDeleted)   ArrWall [NumWall++] = new Wall (wallImage, "Rectangle",  tmpX,  tmpY, EDGE, EDGE, 20, 20);
							else if ((whichWall == triangleW) && !triangleDeleted)     ArrWall [NumWall++] = new Wall (wallImage, "Triangle",  tmpX,  tmpY, EDGE, EDGE, 20, 20);
							else if ((whichWall == circleW) && !circleDeleted)         ArrWall [NumWall++] = new Wall (wallImage, "Circle",  tmpX,  tmpY, EDGE, EDGE, 20, 20);
							else if ((whichWall == saveW) && !saveDeleted)             ArrWall [NumWall++] = new Wall (wallImage, "Save",  tmpX,  tmpY, EDGE, EDGE, 20, 20);
						}
					}
				}	
	}

	void saveLVL_AD (char *tmpC){ //���������� ������ �������
		Coordinate tmp1, tmp2; //��������� ���������� ������ � ������
		tmp1.x = ArrWall [indexStart] -> x;
		tmp1.y = ArrWall [indexStart] -> y;
		tmp2.x = ArrWall [indexFinish] -> x;
		tmp2.y = ArrWall [indexFinish] -> y;
		quickSort (0, NumWall - 1, ArrWall); //���������
		indexStart = binSearch (0, NumWall - 1, ArrWall, tmp1); //����� ���������� ������� ������� ������ � ������
		indexFinish = binSearch (0, NumWall - 1, ArrWall, tmp2);

		ofstream outF (tmpC);
		int tmp = 0;
		outF << NumWall << endl;
		outF << Start.x << " " << Start.y << endl;
		outF << NumLVLDeath << endl;
		outF << ArrWall [indexStart] -> x << " " << ArrWall [indexStart] -> y << " Start" << endl;
		outF << ArrWall [indexFinish] -> x << " " << ArrWall [indexFinish] -> y << " Finish" << endl;
		for (int i = 0; i < NumWall; i++){
			if (i != indexStart && i != indexFinish){
				outF << ArrWall [i] -> x << " " << ArrWall [i] -> y;
				if (ArrWall [i] -> name == "Wall")             outF << " Wall" << endl;
				else if (ArrWall [i] -> name == "Rectangle")   outF << " Rectangle" << endl;
				else if (ArrWall [i] -> name == "Circle")      outF << " Circle" << endl;
				else if (ArrWall [i] -> name == "Triangle")    outF << " Triangle" << endl;
				else if (ArrWall [i] -> name == "Save")        outF << " Save" << endl;
			}
		}
	}

	void saveLVL_PL (char *tmpC){ //���������� ������ ������� (����� �������� ������� ��� ������� � ����, �� ������������ ���� ������� ���� ���� ��� � �������� �������� ������)
		ofstream outF (tmpC);
		int tmp = 0;
		outF << NumWall << endl;
		outF << Start.x << " " << Start.y << endl; //���������� ������, ���� ���������
		outF << NumLVLDeath << endl;
		outF << ArrWall [indexStart] -> x << " " << ArrWall [indexStart] -> y << " Start" << endl;
		outF << ArrWall [indexFinish] -> x << " " << ArrWall [indexFinish] -> y << " Finish" << endl;
		for (int i = 0; i < NumWall; i++){
			if (i != indexStart && i != indexFinish){
				outF << ArrWall [i] -> x << " " << ArrWall [i] -> y;
				if (ArrWall [i] -> name == "Wall")             outF << " Wall" << endl;
				else if (ArrWall [i] -> name == "Rectangle")   outF << " Rectangle" << endl;
				else if (ArrWall [i] -> name == "Circle")      outF << " Circle" << endl;
				else if (ArrWall [i] -> name == "Triangle")    outF << " Triangle" << endl;
				else if (ArrWall [i] -> name == "Save")        outF << " Save" << endl;
			}
		}
	}

	void openLVL_PL (char *tmpName){ //�������� ������ ������
		char tmpC [50];
		strcpy (tmpC, tmpName);
		int tmpX, tmpY;

		for (int i = 0; i < NumWall; i++) //������� ��� ��� ���� �� �����
			ArrWall [i] -> ~Wall ();
		for (int i = 0; i < NUM_CELL_X; i++)
			for (int j = 0; j < NUM_CELL_Y; j++)
				wallCoordinate [i][j] = false;

		ifstream inF (tmpC);
		inF >> NumWall; 
		inF >> Start.x >> Start.y;
		inF >> NumLVLDeath;

		for (int i = 0; i < NumWall; i++){
			inF >> tmpX >> tmpY >> tmpC;
			ArrWall [i] = new Wall (wallImagePL, tmpC, tmpX, tmpY, SQUARE, SQUARE, 40, 40);

			if (strcmp (tmpC, "Wall") == 0)
				wallCoordinate [tmpX][tmpY] = true;
			else if (strcmp (tmpC, "Start") == 0)
				indexStart = i;
			else if (strcmp (tmpC, "Finish") == 0){
				Finish.x = tmpX; Finish.y = tmpY;
				indexFinish = i;
			}
		}

		Coordinate tmp1, tmp2; //��������� ���������� ������ � ������
		tmp1.x = ArrWall [indexStart] -> x;
		tmp1.y = ArrWall [indexStart] -> y;
		tmp2.x = ArrWall [indexFinish] -> x;
		tmp2.y = ArrWall [indexFinish] -> y;
		quickSort (0, NumWall - 1, ArrWall); //���������
		indexStart = binSearch (0, NumWall - 1, ArrWall, tmp1); //����� ���������� ������� ������� ������ � ������
		indexFinish = binSearch (0, NumWall - 1, ArrWall, tmp2);

		createMap (); //������� �����, ��� � ��� ����� ����� ���� �������� ��� ���� ������
		pl -> F_move = false;
	}

	void openLVL_AD (char *tmpName){ //�������� ������ �������
		char tmpC [50];
		strcpy (tmpC, tmpName);
		int tmpX, tmpY;

		for (int i = 0; i < NumWall; i++)
			ArrWall [i] -> ~Wall ();
		for (int i = 0; i < NUM_CELL_X; i++)
			for (int j = 0; j < NUM_CELL_Y; j++)
				wallCoordinate [i][j] = false;

		ifstream inF (tmpC);
		inF >> NumWall; 
		inF >> Start.x >> Start.y;
		inF >> NumLVLDeath;

		for (int i = 0; i < NumWall; i++){
			inF >> tmpX >> tmpY >> tmpC;
			ArrWall [i] = new Wall (wallImage, tmpC, tmpX, tmpY, EDGE, EDGE, 20, 20);

			if (strcmp (tmpC, "Wall") == 0)
				wallCoordinate [tmpX][tmpY] = true;
			else if (strcmp (tmpC, "Start") == 0)
				indexStart = i;
			else if (strcmp (tmpC, "Finish") == 0){
				Finish.x = tmpX; 
				Finish.y = tmpY; indexFinish = i;
			}
		}

		Coordinate tmp1, tmp2; //��������� ���������� ������ � ������
		tmp1.x = ArrWall [indexStart] -> x;
		tmp1.y = ArrWall [indexStart] -> y;
		tmp2.x = ArrWall [indexFinish] -> x;
		tmp2.y = ArrWall [indexFinish] -> y;
		quickSort (0, NumWall - 1, ArrWall); //���������
		indexStart = binSearch (0, NumWall - 1, ArrWall, tmp1); //����� ���������� ������� ������� ������ � ������
		indexFinish = binSearch (0, NumWall - 1, ArrWall, tmp2);
	}

	void inputKeyboard (char *tmpC){ //���� � ����������
		if (event.type == Event::KeyPressed){
			mcText *text;
			Font font;
			font.loadFromFile ("Resources/Fonts/modeka.otf");
			text = new mcText (&font); //������� ����� ������� ����� ������������ �� ������
			text -> changeSize (SIZE_TEXT); //������ ������
			text -> add (tmpC);
			if (Keyboard::isKeyPressed (Keyboard::BackSpace)){ //���� ������ ������ �������     
				int i = strlen (tmpC);
				if (i > 0){
					char tmpC2 [50];
					strcpy (tmpC2, tmpC);
					strncpy (tmpC, tmpC2, i - 1);
					tmpC [i - 1] = '\0';
				}
			}
			else if (text -> w < (W_BUTTON - SIZE_TEXT)){
				char tmpC2 [2];
				if (event.key.code >= 0 && event.key.code <= 25){ //����� �� � �� Z
					tmpC2 [0] = event.key.code + 97;
					tmpC2 [1] = '\0';
					strcat (tmpC, tmpC2);
				}
				else if (event.key.code >= 26 && event.key.code <= 35){ //����� �� 0 �� 9
					tmpC2 [0] = event.key.code + 22;
					tmpC2 [1] = '\0';
					strcat (tmpC, tmpC2);
				}
			}
		}
			
	}

	void createWay (){ //������� �������� ������ �� ���������
		Coordinate fn, sizeMap, st;
		st.x = Start.x;
		st.y = Start.y;
		fn.x = Finish.x;
		fn.y = Finish.y;
		pl -> currDirection = 0;
		sizeMap.x = NUM_CELL_X;
		sizeMap.y = NUM_CELL_Y;
		outputSearch (wallCoordinate, fn, st, sizeMap);
	}

	void changeState (){ //������� ��������� ���������
		if (!F_secPhaseChangeState){ //������ ���� (���������� ������)
			bool tmpB = true;
			for (int i = 0; i < NumButton; i++)
				if (button [i] -> subtype == whichStateWas){
					if (button [i] -> F_transformation == false && tmpB){ //������a�� ���a ������� �� ��������� ���� ������
						F_secPhaseChangeState = true; F_reduceWholeType = false; //������� � ���������� ������
						subtype = whichStateWill; type = findType (subtype);
						for (int i = 0; i < NumButton; i++) //��������� �������� ������ ������� ����, � ������ ������ ������� ����� ������������
							if (button [i] -> subtype == whichStateWill){
								button [i] -> F_draw  = true;
								button [i] -> clear ();
							}
							else
								button [i] -> F_draw  = false;

						if (whichStateWas == listLVL){ //�� ������� ����������� ������, ���� �� ������� ����� ��������� ������ (����� ��� ������ ������� � ����� ����)
							for (int k = NumButton - 1; k > NumButton - NumListLVL - 1; k--)
								delete button [k];
							NumButton -= NumListLVL;
							NumListLVL = 0;
						}
					}

					button [i] -> EFF_reduce ();
				}
				else if (F_reduceWholeType && button [i] -> subtype == wholeType)
					button [i] -> EFF_reduce (); 
		}
		else{ //����� ���� (���������� ������)
			bool tmpB = true;
			for (int i = 0; i < NumButton; i++){
				if (button [i] -> subtype == whichStateWill){
					button [i] -> EFF_enlarge (); 
					if (button [i] -> F_transformation == false && tmpB){ //����������� ���� ���� ������� �� ���������
						F_changeStates = false; F_secPhaseChangeState = false; F_enlargeWholeType = false; tmpB = false;
						for (int i = 0; i < NumButton; i++)
							if (button [i] -> subtype == subtype)
								button [i] -> F_draw = true;
							else
								button [i] -> F_draw = false;
					}
				}
				else if (F_enlargeWholeType && button [i] -> subtype == wholeType)
					button [i] -> EFF_enlarge (); 
			}
		}
	}

	void startChangeState (SubtypeState tmpS){ //��������������� ������� ��� ��������� ���������
		sndClickButt.play (); //������������� ���� ����� �� ������
		F_changeStates = true; 
		whichStateWas = subtype; whichStateWill = tmpS;
		TypeState tmpT = findType (tmpS);
		if ((tmpT == player && type == unknown) || (tmpT == player && type == menu)) //����� ��� ������ ������� ����� ����� (����������, ����� � ������, ��� � ��� �� ������ ��������� ����� �� ������� � �����, � ���� ����� �� ������� �� ���� ������)
			F_enlargeWholeType = true;
		else if (tmpT == menu && type == player)
			F_reduceWholeType = true;
		for (int i = 0; i < NumButton; i++){
			if (button [i] -> subtype == whichStateWas)
				button [i] -> F_transformation = true;
			else if (button [i] -> subtype == whichStateWill)
				button [i] -> F_transformation = true;
			else if (button [i] -> subtype == wholeType && F_enlargeWholeType){
				button [i] -> F_transformation = true; button [i] -> F_draw = true; 
				button [i] -> clear ();
			}
			else if (button [i] -> subtype == wholeType && F_reduceWholeType)
				button [i] -> F_transformation = true;
		}
	}

	void createMap (){ //������� �������� �����
		for (int i = 0; i < (NUM_CELL_X + 2) * (NUM_CELL_Y + 2); i++)
			levelArray [i] = 7;
		int tmpI;
		for (int i = 0; i < NumWall; i++){
			if (ArrWall [i] -> name == "Save")             tmpI = 0;
			else if (ArrWall [i] -> name == "Wall")        tmpI = 1;
			else if (ArrWall [i] -> name == "Finish")      tmpI = 2;
			else if (ArrWall [i] -> name == "Start")       tmpI = 3;
			else if (ArrWall [i] -> name == "Circle")      tmpI = 4;
			else if (ArrWall [i] -> name == "Rectangle")   tmpI = 5;
			else if (ArrWall [i] -> name == "Triangle")    tmpI = 6;	
			levelArray [1 + ArrWall [i] -> x + (ArrWall [i] -> y + 1) * (NUM_CELL_X + 2)] = tmpI; //��������� ������ � ������� �������� �������
		}
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < NUM_CELL_X + 2; j++)
				levelArray [i * (NUM_CELL_Y + 1) * (NUM_CELL_X + 2) + j] = 1;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < NUM_CELL_Y + 2; j++)
				levelArray [i * (NUM_CELL_X + 1) + j * (NUM_CELL_X + 2)] = 1;

		
		map.load("Resources/Textures/wall2.png", Vector2u (40, 40), levelArray, NUM_CELL_X + 2, NUM_CELL_Y + 2, Vector2i (SQUARE, SQUARE));
		map.setOrigin ((float) SQUARE * Start.x + 3 * SQUARE / 2, (float) SQUARE * Start.y + 3 * SQUARE / 2);
		map.setPosition ((float) W_WIN / 2, (float) H_WIN / 2);
	}


	void StateLauncher (){
		if (F_changeStates) //�� ������ ����� �������� ������ ����� �������� ���������, ��� �����  ������������ � ������� ����. �� �������� �� ��� ������
			changeState ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> F_click && button [i] -> name == "Mode" && !F_changeStates){
					startChangeState (mode);
					break;
				}
				else if (((button [i] -> F_click && button [i] -> name == "Go!") || F_enterReleased) && !F_changeStates){
					if (AdOrPlMode == "AdminMode"){
						NumWall = 0; //������ � ����� ������ ����� ����� � �����. �� ������ � ������ ����� ������
						ArrWall [NumWall++] = new Wall (wallImage, "Start", 0, 0, EDGE, EDGE, 20, 20);
						ArrWall [NumWall++] = new Wall (wallImage, "Finish", 1, 0, EDGE, EDGE, 20, 20);
						indexStart = 0;
						indexFinish = 1;
						whichWall = wall;
						startChangeState (editLVL);
						backgroundMusic.pause (); //� ����� ���� ������ �� ������
					}
					if (AdOrPlMode == "PlayerMode"){  
						writeInfo ();
						startChangeState (selectLVL);
					}
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "Settings" && !F_changeStates){
					startChangeState (settings); break;
				}
				else if (((button [i] -> F_click && button [i] -> name == "Exit") || F_escapeReleased) && !F_changeStates){
					writeInfo ();
					if (!F_gameIsLocked)
						startChangeState (exitt); 
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "About" && !F_changeStates){
					startChangeState (about); break;
				}
			}
	}
	void StateMode (){
		if (F_changeStates)
			changeState ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToMenu") || F_escapeReleased) && !F_changeStates){
					startChangeState (launcher);
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "AdminMode" && !F_changeStates){
					 sndClickButt.play ();
					 if (passedLVL >= NUM_LVL - 2) //����� ��� �������� ���� �� ����������� 2 �������
						 AdOrPlMode = "AdminMode"; 
					 else
						 message.showMessage (W_WIN / 2, H_WIN / 2 - 4 * (H_BUTTON + INTERVAL), "Finish 2 levels", 0.8f);
					 break;
				}
				else if (button [i] -> F_click && button [i] -> name == "PlayerMode" && !F_changeStates){
					sndClickButt.play (); AdOrPlMode = "PlayerMode"; break;
				}
			}
	}
	void StateSettings (){
		if (F_changeStates)
			changeState ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToMenuSet") || F_escapeReleased) && !F_changeStates){
					writeInfo ();
					if (!F_inSetingIntoPause)
						startChangeState (launcher);
					else{
						startChangeState (pause);
						F_inSetingIntoPause = false;
					}
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "ControlsSet" && !F_changeStates){
					char tmpC [2];

					for (int k = 0; k < NumButton; k++)
						if (button [k] -> name == "ChangeKey"){
							if (key [button [k] -> value - 1] >= 0 && key [button [k] -> value - 1] <= 25){
								tmpC [0] = key [button [k] -> value - 1] + 65;
								tmpC [1] = '\0';
							}
							else if (key [button [k] -> value - 1] >= 26 && key [button [k] -> value - 1] <= 35){
								tmpC [0] = key [button [k] -> value - 1] + 22;
								tmpC [1] = '\0';
							}
							button [k] -> updateText (tmpC); //����� ������ � ��������� ����������, �������� �� �������� ����, �� ����� ������ �� ��� ���� ������ � �������� ����� �� �������
						}
					startChangeState (controlSeting); 
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "AudioSet" && !F_changeStates){
					startChangeState (audioSetting); break;
				}
			}
	}
	void StateExitt (){
		if (F_changeStates)
			changeState ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "QuitNo") || F_escapeReleased) && !F_changeStates){
					startChangeState (launcher); break;
				}
				else if (((button [i] -> F_click && button [i] -> name == "QuitYes") || F_enterReleased) && !F_changeStates){
					sndClickButt.play (); 
					window -> close (); 
					break;
				}
			}
	}
	void StateSelectLVL (){
		if (F_changeStates)
			changeState ();
		char tmpC2 [30];
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> F_click && button [i] -> name == "SelectLVL" && !F_changeStates){
					CurrentLVL = button [i] -> value;
					_itoa (button [i] -> value, tmpC2, 10);

					char nameFile [30] = "Resources/LVLs/lvl";
					strcat (nameFile, tmpC2);
					strcat (nameFile, ".prof");
					strcpy (lvlOpenByPlayer, nameFile);

					F_isPlayerLVL = false;
					timer = 0;
					startChangeState (loadingLVL);	
				}
				else if (((button [i] -> F_click && button [i] -> name == "BackToMenuSel") || F_escapeReleased) && !F_changeStates){
					startChangeState (launcher); break;
				}
				else if (button [i] -> F_click && button [i] -> name == "My lvls" && !F_changeStates){
					readInfo ();
					startChangeState (playerLVL); break;
				}
			}
	}
	void StateAudioSeting (){
		if (F_changeStates)
			changeState ();	

		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToSetAudio") || F_escapeReleased) && !F_changeStates){
					writeInfo ();
					startChangeState (settings);
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "MusicSlider" && !F_changeStates){
					sndClickButt.play (); break;
				}
				else if (button [i] -> F_click && button [i] -> name == "SoundSlider" && !F_changeStates){
					sndClickButt.play (); break;
				}
				else if (button [i] -> F_click && button [i] -> name == "SwitchMusic" && !F_changeStates){
					sndClickButt.play ();
					if (F_musicOff) changeMusicVolume (0); //���� ��������� ������
					else            changeMusicVolume (volumeMusic);
					break;
				}
				else if (button [i] -> F_click && button [i] -> name == "SwitchSound" && !F_changeStates){
					sndClickButt.play (); 
					if (F_soundOff) changeSoundVolume (0); //���� ��������� �����
					else            changeSoundVolume (volumeSound);
					break;
				}
			}
	}
	void StateControlSeting (){
		if (F_changeStates)
			changeState ();

		if (F_anyKeyReleased && F_changeKey){
			for (int i = 0; i < NumButton; i++)
				if (button [i] -> name == "ChangeKey" && button [i] -> value == whatButChange){
					char tmpC [2];
					bool tmpB = true;

					for (int k = 0; k < 3; k++) //���� �� ����� ��������� ��������� ����� �� ���� ������ (��� ������)
						if (key [k] == event.key.code && k != button [i] -> value - 1)
							tmpB = false;

					if (event.key.code >= 0 && event.key.code <= 25 && tmpB){ //����� ����� ������� ���� ������, � ������ ����� �� ������
						tmpC [0] = event.key.code + 65;
						tmpC [1] = '\0';
						button [i] -> updateText (tmpC);
						whatButChange = -1;
						key [button [i] -> value - 1] = event.key.code;
						button [indexControlBut [button [i] -> value - 1]] -> updateText (tmpC);
					}
					else if (event.key.code >= 26 && event.key.code <= 35 && tmpB){
						tmpC [0] = event.key.code + 22;
						tmpC [1] = '\0';
						button [i] -> updateText (tmpC);
						whatButChange = -1;
						key [button [i] -> value - 1] = event.key.code;
						button [indexControlBut [button [i] -> value - 1]] -> updateText (tmpC);
					}
					break;
				}
		}

		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToControlSet") || F_escapeReleased) && !F_changeStates){
					writeInfo ();
					whatButChange = -1;
					startChangeState (settings);
					break;
				}
				if (button [i] -> F_click && button [i] -> name == "ChangeKey" && !F_changeStates){
					F_changeKey = true;
					sndClickButt.play ();
					whatButChange = button [i] -> value;
					break;
				}
			}
	}
	void StatePlayerLVL (){
		if (F_changeStates)
			changeState ();
		inputKeyboard (playerLVLOpenByPlayer);
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> name == "InputMyLVL" && !F_changeStates)
						button [i] -> updateText (playerLVLOpenByPlayer);
				if (((button [i] -> F_click && button [i] -> name == "InputMyLVL") || F_enterReleased) && !F_changeStates){
					sndClickButt.play (); 
					char tmpC [100] = "Resources/LVLs/";
					bool findLVL = false;
					ifstream inF ("Resources/LVLs/listLVLs.prof");
					char tmpC2 [30];
					int tmpI;
					inF >> tmpI;
					for (int j = 0; j < tmpI; j++){
						inF >> tmpC2;
						if (strcmp (tmpC2, playerLVLOpenByPlayer) == 0){
							if (strstr (playerLVLOpenByPlayer, "lvl") == NULL || strpbrk (playerLVLOpenByPlayer, "1234") == NULL || strlen (playerLVLOpenByPlayer) > 4){
								strcat (tmpC, playerLVLOpenByPlayer); //��������� �� ����������� �� ������ ����� ������� �����
								strcat (tmpC, ".prof");
								strcpy (lvlOpenByPlayer, tmpC);
								

								timer = 0;
								F_isPlayerLVL = true;
								findLVL = true;
								startChangeState (loadingLVL);
							}
							break;
						}
					}

					if (!findLVL){
						for (int k = 0; k < NumButton; k++)
							if (button [k] -> name == "InputMyLVL"){
								strcpy (playerLVLOpenByPlayer, "");
								break;
							}
						message.showMessage (W_WIN / 2, H_WIN / 2 - 4 * (H_BUTTON + INTERVAL), "Level not found", 0.7f);
					}
				}
				else if (((button [i] -> F_click && button [i] -> name == "BackToMenuMyLVL") || F_escapeReleased) && !F_changeStates)
					startChangeState (selectLVL);
			}
	}
	void StateAbout (){
		if (F_changeStates)
			changeState ();

		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToMenuAbout") || F_escapeReleased) && !F_changeStates){
					startChangeState (launcher);
					break;
				}
			}
	}

	void StateEditLVL (){
		timer += time;
		if (F_changeStates)
			changeState ();
		createWalls ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> F_click && button [i] -> name == "SaveAd" && !F_changeStates){
					startChangeState (saveLVL); break;
				}
				else if (button [i] -> F_click && button [i] -> name == "OpenAd" && !F_changeStates){
					startChangeState (openLVL); break;
				}
				else if (button [i] -> F_click && button [i] -> name == "DeleteAd" && !F_changeStates){
					startChangeState (deleteLVL); break;
				}
				else if (button [i] -> F_click && button [i] -> name == "ListAd" && !F_changeStates){
					Font font;
					font.loadFromFile ("Resources/Fonts/modeka.otf");
					ifstream inF ("Resources/LVLs/listLVLs.prof");
					char tmpC [50];
					int tmpI;
					SubtypeState tmpS = listLVL;
					inF >> tmpI;
					for (int i = 0; i < tmpI; i++){
						inF >> tmpC; //��������� ����� ������ ������ ���� � ������, � ������ ������ (����� ��� ����� ��������� �� ���� ����� �������
						if (strstr (tmpC, "lvl") == NULL || strpbrk (tmpC, "1234") == NULL || strlen (tmpC) > 4){
							button [NumButton++] = new Static (tmpC, "ListLVL", font, tmpS, W_WIN / 2, H_WIN / 2 + NUM_CELL_Y * EDGE / 2 - H_BUTTON * (NumListLVL + 1));
							NumListLVL++;
						}
					}
					button [NumButton++] = new Static ("List of LVLs:", "ListLVL", font, tmpS, W_WIN / 2, H_WIN / 2 + NUM_CELL_Y * EDGE / 2 - H_BUTTON * (NumListLVL + 1));
					NumListLVL++;

					startChangeState (listLVL); 

					break;
				}
				else if (((button [i] -> F_click && button [i] -> name == "BackToMenuAd") || F_escapeReleased) && !F_changeStates){
					startChangeState (launcher); timer = 0; 
					backgroundMusic.play ();
					break;
				}
			}

			if (event.type == Event::KeyPressed){
				if (Keyboard::isKeyPressed (Keyboard::Num1))        whichWall = rectangleW;
				else if (Keyboard::isKeyPressed (Keyboard::Num2))   whichWall = triangleW;
				else if (Keyboard::isKeyPressed (Keyboard::Num3))   whichWall = circleW;
				else if (Keyboard::isKeyPressed (Keyboard::Num4))   whichWall = wall;
				else if (Keyboard::isKeyPressed (Keyboard::Num5))   whichWall = startW;
				else if (Keyboard::isKeyPressed (Keyboard::Num6))   whichWall = finishW;
				else if (Keyboard::isKeyPressed (Keyboard::Num7))   whichWall = saveW;
			}
	}
	void StateOpenLVL (){
		if (F_changeStates)
			changeState ();
		inputKeyboard (lvlOpenByAdmin);
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> name == "EditLVL" && !F_changeStates)
					button [i] -> updateText (lvlOpenByAdmin);
				if (((button [i] -> F_click && button [i] -> name == "EditLVL") || F_enterReleased) && !F_changeStates){
					bool findLVL = false;
					char tmpC [100] = "Resources/LVLs/";
					ifstream inF ("Resources/LVLs/listLVLs.prof");
					char tmpC2 [30];
					int tmpI;
					inF >> tmpI;
					for (int i = 0; i < tmpI; i++){
						inF >> tmpC2;
						if (strcmp (tmpC2, lvlOpenByAdmin) == 0){
							if (strstr (lvlOpenByAdmin, "lvl") == NULL || strpbrk (lvlOpenByAdmin, "1234") == NULL || strlen (lvlOpenByAdmin) > 4){
								strcat (tmpC, lvlOpenByAdmin);
								strcat (tmpC, ".prof");
								openLVL_AD (tmpC);
								startChangeState (editLVL);
								findLVL = true;
								break;
							}
							else{ //���� ����� ����� ������� ����������� ������
								sndClickButt.play ();
								for (int k = 0; k < NumButton; k++)
									if (button [k] -> name == "EditLVL"){
										strcpy (lvlOpenByAdmin, "");
										message.showMessage (W_WIN / 2 + 2 * W_BUTTON, button [k] -> y, "this level may not be opened", 1.5f);
										break;
									}
							}
						}
					}
					if (!findLVL){ //���� �� ����� �������
						sndClickButt.play ();
						for (int k = 0; k < NumButton; k++)
							if (button [k] -> name == "EditLVL"){
								strcpy (lvlOpenByAdmin, "");
								message.showMessage (W_WIN / 2 + 2 * W_BUTTON, button [k] -> y, "Level not found", 0.7f);
								break;
							}
					}
				}
				else if (((button [i] -> F_click && button [i] -> name == "BackToAdminSel") || F_escapeReleased) && !F_changeStates)
					startChangeState (editLVL);
			}
	}
	void StateSaveLVL (){
		if (F_changeStates)
			changeState ();
		inputKeyboard (lvlOpenByAdmin);
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> name == "AdSaveLVL" && !F_changeStates)
					button [i] -> updateText (lvlOpenByAdmin);
				if (((button [i] -> F_click && button [i] -> name == "AdSaveLVL") || F_enterReleased) && !F_changeStates){
					int tmpI; 
					char tmpC2 [100][30]; 
					bool edit = false;
					ifstream inF ("Resources/LVLs/listLVLs.prof");
					inF >> tmpI;
					for (int i = 0; i < tmpI; i++){
						inF >> tmpC2 [i];
						if (strcmp (tmpC2 [i], lvlOpenByAdmin) == 0)
							edit = true;
					}
					inF.close ();
					if (strstr (lvlOpenByAdmin, "lvl") == NULL || strpbrk (lvlOpenByAdmin, "1234") == NULL || strlen (lvlOpenByAdmin) > 4){
						ofstream outF ("Resources/LVLs/listLVLs.prof");
						if (!edit){ //���� ������� ����� �������
							if (tmpI - NUM_LVL < 8){
								outF << ++tmpI << endl;
								for (int i = 0; i < tmpI - 1; i++)
									outF << tmpC2 [i] << endl;
								outF << lvlOpenByAdmin << endl;
								NumLVLDeath = 0;
								char tmpC [100] = "Resources/LVLs/";
								strcat (tmpC, lvlOpenByAdmin);
								strcat (tmpC, ".prof");
								Start.x = ArrWall [indexStart] -> x;
								Start.y = ArrWall [indexStart] -> y;
								saveLVL_AD (tmpC);
								startChangeState (editLVL);
							}
							else{ //����� ����� ������� ���� 8 �������, ���� �� ���������� ������
								outF << tmpI << endl;
								for (int k = 0; k < tmpI; k++)
									outF << tmpC2 [k] << endl;
								sndClickButt.play ();
								for (int k = 0; k < NumButton; k++)
									if (button [k] -> name == "AdSaveLVL"){
										strcpy (lvlOpenByAdmin, "");
										message.showMessage (W_WIN / 2 + 2 * W_BUTTON, button [k] -> y, "You can't create more LVL", 0.7f);
										break;
									}
							}
						}
						else{ //����������� ������ �������
							outF << tmpI << endl;
							for (int k = 0; k < tmpI; k++)
								outF << tmpC2 [k] << endl;
							char tmpC [100] = "Resources/LVLs/";
							strcat (tmpC, lvlOpenByAdmin);
							strcat (tmpC, ".prof");
							Start.x = ArrWall [indexStart] -> x;
							Start.y = ArrWall [indexStart] -> y;
							saveLVL_AD (tmpC);
							startChangeState (editLVL);
						}	
					}
					else{ //���� ����� ����� ��������� ������� �� ��� ������������
						sndClickButt.play ();
						for (int k = 0; k < NumButton; k++)
							if (button [k] -> name == "AdSaveLVL"){
								strcpy (lvlOpenByAdmin, "");
								message.showMessage (W_WIN / 2 + 2 * W_BUTTON, button [k] -> y, "Enter other name", 1.5f);
								F_enterReleased = false;
								break;
							}
					}
				}
				else if (((button [i] -> F_click && button [i] -> name == "BackToAdminSave") || F_escapeReleased) && !F_changeStates)
					startChangeState (editLVL);
			}
	}
	void StateDeleteLVL (){
		if (F_changeStates)
			changeState ();
		inputKeyboard (lvlOpenByAdmin);
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (button [i] -> name == "AdDeleteLVL" && !F_changeStates)
					button [i] -> updateText (lvlOpenByAdmin);
				if (((button [i] -> F_click && button [i] -> name == "AdDeleteLVL") || F_enterReleased) && !F_changeStates){
					if (strstr (lvlOpenByAdmin, "lvl") == NULL || strpbrk (lvlOpenByAdmin, "1234") == NULL || strlen (lvlOpenByAdmin) > 4){
						int tmpI; 
						char tmpC2 [100][30]; 
						bool isDelete = false;
						ifstream inF ("Resources/LVLs/listLVLs.prof");
						inF >> tmpI;
					
						for (int j = 0; j < tmpI; j++){
							inF >> tmpC2 [j];
							if (strcmp (tmpC2 [j], lvlOpenByAdmin) == 0)
								isDelete = true;
						}
						inF.close ();

						if (isDelete){
							char tmpC [50];
							strcpy (tmpC, "Resources/LVLs/");
							strcat (tmpC, lvlOpenByAdmin);
							strcat (tmpC, ".prof");
							remove (tmpC);
							ofstream outF ("Resources/LVLs/listLVLs.prof");
							outF << tmpI - 1 << endl;
							for (int i = 0; i < tmpI; i++)
								if (strcmp (tmpC2 [i], lvlOpenByAdmin) != 0)
									outF << tmpC2 [i] << endl;
							startChangeState (editLVL);
						}
						else{ //���� �� ����� ������� ������� ����� ������� �����
							sndClickButt.play ();
							for (int k = 0; k < NumButton; k++)
								if (button [k] -> name == "AdDeleteLVL"){
									strcpy (lvlOpenByAdmin, "");
									message.showMessage (W_WIN / 2 + 2 * W_BUTTON, button [k] -> y, "Level not found", 0.7f);
									break;
								}
					
						}
					}
					else{ //������ ������� ����������� ������
						sndClickButt.play ();
						for (int k = 0; k < NumButton; k++)
							if (button [k] -> name == "AdDeleteLVL"){
								strcpy (lvlOpenByAdmin, "");
								message.showMessage (W_WIN / 2 + 2 * W_BUTTON, button [k] -> y, "this level may not be deleted", 1.5f);
								break;
							}
					}
				}
				else if (((button [i] -> F_click && button [i] -> name == "BackToAdminDelete") || F_escapeReleased) && !F_changeStates)
					startChangeState (editLVL);
			}
	}
	void StateListLVL (){
		if (F_changeStates)
			changeState ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToAdminList") || F_escapeReleased) && !F_changeStates){
					startChangeState (editLVL); break;
					//�� ������� ����� ������ ��������� ��� ������, ������ ��� ��� ������ ��� ����������
				}
			}
	}
	
	void StatePlay (){
		if (F_changeStates)
			changeState (); 
		if (!F_changeStates) //������ ���, ��� else!!
			pl -> update ();
			

		if (map.getPosition ().x == W_WIN / 2 && map.getPosition ().y == H_WIN / 2){ //�������� �� ����� ������� �� ������ ��� �� ����
			if (!F_changeStates && !pl -> F_move && !pl -> F_reduce){
				if ((levelArray [1 + pl -> x + (pl -> y + 1) * (NUM_CELL_X + 2)] == 5 && pl -> state != rectangle) || (levelArray [1 + pl -> x + (pl -> y + 1) * (NUM_CELL_X + 2)] == 4 && pl -> state != circle) || (levelArray [1 + pl -> x + (pl -> y + 1) * (NUM_CELL_X + 2)] == 6 && pl -> state != triangle)){
					if (!pl -> F_teleportation){ //���� ����� ������ �� �� �������������� ��� ������ �� ���������� ��� �� �����
						sndTeleport.play (); timer = 0;
					}
					pl -> EFF_teleportation (Start.x, Start.y); 
					timer += time;

					if (!pl -> F_teleportation){
						map.setOrigin ((float) SQUARE * Start.x + 3 * SQUARE / 2, (float) SQUARE * Start.y + 3 * SQUARE / 2); //�������� �����
						map.setPosition ((float) W_WIN / 2, (float) H_WIN / 2);

						startChangeState (startLVL);
						NumLVLDeath++;
						char tmpC [30]; //���������� ���������� �������
						button [indexDeathPlayerBut] -> updateText (_itoa (NumLVLDeath, tmpC, 10));
						createWay ();
					}
				}
				else if (levelArray [1 + pl -> x + (pl -> y + 1) * (NUM_CELL_X + 2)] == 0){ //���� ������� �� ����
					Start.x = pl -> x;
					Start.y = pl -> y;
				}
			}
		}

		if (!F_changeStates){
			char tmpC [30]; //���������� �������
			button [indexTimePlBut] -> updateText (_itoa ((int) lvlTime, tmpC, 10));
			lvlTime += time;
		}

		for (int i = 0; i < NumButton; i++)
			if (button [i] -> F_draw){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "BackToMenuPl") || F_escapeReleased) && !F_changeStates){
					lvlTime -= time;
					F_escapeReleased = false;
					startChangeState (pause);
				}
				else if (F_lvlComplete && !F_changeStates){ //���� ������� ��������
					AllTime += lvlTime;
					lvlTime = 0;
					sndClickButt.play (); 
					F_lvlComplete = false;
					if (!F_isPlayerLVL){
						if (CurrentLVL < NUM_LVL){
							if (passedLVL < NUM_LVL && CurrentLVL - 1 == passedLVL)
								passedLVL++;
							writeInfo ();
							CurrentLVL++; 
							Start.x = ArrWall [indexStart] -> x;
							Start.y = ArrWall [indexStart] -> y;
							saveLVL_PL (lvlOpenByPlayer);
							char tmpC [40], *tmpC2;
							tmpC2 = _itoa (CurrentLVL, tmpC, 10);
							char nameFile [40] = "Resources/LVLs/lvl";
							strcat (nameFile, tmpC2);
							strcat (nameFile, ".prof");
							strcpy (lvlOpenByPlayer, nameFile);
							
							pl -> F_reduce = false; //��������� ��������� �������
							startChangeState (loadingLVL);
						}
						else{ //���� �� ������ ��������� �������
							pl -> F_reduce = false;
							Start.x = ArrWall [indexStart] -> x;
							Start.y = ArrWall [indexStart] -> y;
							saveLVL_PL (lvlOpenByPlayer);
							passedLVL = NUM_LVL;
							writeInfo ();
							startChangeState (selectLVL);
						}
					}
					else{ //���� �� ������ ������� ��������� �������, �� ��� ������ � ����� ������
						F_isPlayerLVL = false;
						pl -> F_reduce = false;
						Start.x = ArrWall [indexStart] -> x;
						Start.y = ArrWall [indexStart] -> y;
						saveLVL_PL (lvlOpenByPlayer);
						startChangeState (selectLVL);
					}
				}
			}
	}
	void StatePause (){
		if (F_changeStates)
			changeState ();
		for (int i = 0; i < NumButton; i++)
			if (button [i] -> subtype == pause){
				button [i] -> checkCursor ();
				if (((button [i] -> F_click && button [i] -> name == "LeaveToSel") || F_enterReleased) && !F_changeStates){
					AllTime += lvlTime;
					lvlTime = 0;
					writeInfo ();
					saveLVL_PL (lvlOpenByPlayer);
					startChangeState (selectLVL);
					F_lvlComplete = false;
					break;
				}
				if (((button [i] -> F_click && button [i] -> name == "BackToPlPause") || F_escapeReleased) && !F_changeStates){
					startChangeState (play); break;
				}
				if (((button [i] -> F_click && button [i] -> name == "SettingsIntoPause") || F_escapeReleased) && !F_changeStates){
					startChangeState (settings); F_inSetingIntoPause = true; break;
				}
			}
	}
	void StateStartLVL (){
		if (F_changeStates)
			changeState ();
		pl -> changeFigureKey ();
		F_loadLVL = false;
		pl -> EFF_enlarge ();
		if (F_anyKeyReleased && !F_changeStates && !F_escapeReleased){
			if (event.key.code != key [0] && event.key.code != key [1] && event.key.code != key [2] && !pl -> F_enlarge)
				startChangeState (play);
		}
		else if (F_escapeReleased && !F_changeStates){
			AllTime += lvlTime; lvlTime = 0;
			writeInfo ();
			saveLVL_PL (lvlOpenByPlayer); 
			startChangeState (selectLVL);
			F_lvlComplete = false;
		}
	}

	void StateLoadingLVL (){
		if (F_changeStates)
			changeState ();

		timer += time;
		if (CurrentLVL == 4 && !F_changeStates)
			if (timer > 0.1f && timer < 0.5f) //���� ��� ��������� �������, �� ������ �����)
				message.showMessage (W_WIN / 2, H_WIN / 2 + H_BUTTON + INTERVAL, "Good Luck!", 4.5f);

		if (timer >= 1.5f && timer < 2.2f && !F_changeStates){
			if (!F_loadLVL){
				F_loadLVL = true;
				openLVL_PL (lvlOpenByPlayer);
				char tmpC2 [30]; //���������� ������� � ���������� �������
				button [indexDeathPlayerBut] -> updateText (_itoa (NumLVLDeath, tmpC2, 10));
				button [indexDeathPlayerBut] -> clear ();
				button [indexTimePlBut] -> updateText ("0");
				button [indexTimePlBut] -> clear ();
				pl -> changeLocation (Start.x, Start.y);
				pl -> F_enlarge = true;
				pl -> state = rectangle; //��������� ������� � ����������
				pl -> changeFigureStatic ();
				createWay ();
			}
		}
		else if (timer >= 2.5f){ //������ ��������� �� ������ ����
			timer = 0;
			startChangeState (startLVL);
		}
	}


	void update (){ //������� ������� ���������� ����
		switch (subtype){
		case launcher:
			StateLauncher ();
			break;
		case mode:
			StateMode ();
			break;
		case settings:
			StateSettings ();
			break;
		case exitt:
			StateExitt ();
			break;
		case selectLVL:
			StateSelectLVL ();
			break;
		case audioSetting:
			StateAudioSeting ();
			break;
		case controlSeting:
			StateControlSeting ();
			break;
		case playerLVL:
			StatePlayerLVL ();
			break;
		case about:
			StateAbout ();
			break;
		
		case editLVL:
			StateEditLVL ();
			break;
		case openLVL:
			StateOpenLVL ();
			break;
		case saveLVL:
			StateSaveLVL ();
			break;
		case deleteLVL:
			StateDeleteLVL ();
			break;
		case listLVL:
			StateListLVL ();
			break;

		case play:
		//	StatePlay ();
			break;
		case pause:
			StatePause ();
			break;
		case startLVL:
			StateStartLVL ();
			break;

		case loadingLVL:
			StateLoadingLVL ();
			break;
		}
	}
};


int main (){
	System system;
	view.reset (FloatRect (0, 0, (float) system.W_WIN, (float) system.H_WIN)); //�������� ������
	setCoordinateForView ((float) system.W_WIN / 2, (float) system.H_WIN / 2); //������� �� ����� ������
	vector <VideoMode> modes = VideoMode::getFullscreenModes (); //��������� ��� ��������� ����� ���� ��� ������ ������
	VideoMode mode = modes [0]; //�������� ������, ������� � ����� ������� ���������
	
	Game game;
	system.window = new RenderWindow (mode, "Figure", Style::Fullscreen, ContextSettings (0, 0, 1)); //�������� ����
	system.window -> setMouseCursorVisible (false); //�� ������ ������
	bool isUpdate = false; //����, ������� ���������� ����������� �� ����

	while (system.window -> isOpen ()){
		system.time = system.clock.getElapsedTime().asSeconds(); //����� ������ ��� �����������
		system.clock.restart ().asSeconds ();
		system.FPS = (int) (1.0 / system.time); //�������� ��� ��������
		char tmpC [30];
		_itoa (system.FPS, tmpC, 10);
		game.button [game.indexFPSBut] -> updateText (tmpC); //��������� ������, ������� ������� �������� ���

		if (!system.F_gameIsLocked){
			system.mousePosWin = Mouse::getPosition (system.window [0]); //���������� ���� �����. ����
			system.posMouse = system.window -> mapPixelToCoords (system.mousePosWin); //���������� ���� �����. �����
		}


		while (system.window -> pollEvent (system.event)){ //������� � ���� ����� ���-�� ���������� (����� ������� ������ ��� �������� �� �������)
			if (system.event.type == Event::Closed) //���� ����������� ����� ������ �������
				system.window -> close ();
			isUpdate = true;
			game.update (); //��������� ����

			if (system.event.type == Event::KeyReleased && system.event.key.code == 36) game.F_escapeReleased = true; //����, � ��� ����� �� ������ (���� ���� ����� ������ ���, �� ������ �� ������, � ���������, ����������� ���� ��� ����� false � � ������� ��� ���������� ������ ����� ����� �� ���� ����������)
			else game.F_escapeReleased = false;
			if (system.event.type == Event::KeyReleased && system.event.key.code == 58) game.F_enterReleased = true; //����, � ��� ����� �� �����
			else game.F_enterReleased = false;
			if (system.event.type == Event::KeyReleased) game.F_anyKeyReleased = true; //����, � ��� ������ �� �����-���� ������
			else game.F_anyKeyReleased = false;

			if (system.event.type == Event::KeyPressed) //����� ��� ��������� ����� ������, ��� � �������� ��� ������� �������, � �� ����������
				system.keyCodePressed = system.event.key.code;
		}
		if (!isUpdate){ //��������� ���� ���� �� ����� � ���������� while (������ ��� ���� ��������� ����, ���� ���� ����� �� ������� ������ � �� �������� �� �������)
			game.update ();
			game.F_escapeReleased = false;
			game.F_enterReleased = false;
			game.F_anyKeyReleased = false;
		}
		
		isUpdate = false;
		if (game.subtype == play) //������ ��������� ��������� ������
			game.StatePlay ();
		system.keyCodePressed = -1;

		game.draw (); //������ ��
	}
	return 0;
}