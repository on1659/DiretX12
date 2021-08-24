#pragma once

class CSceneLoading_2D: public CGameState_2D
{
private:
	CSpriteObject_2D		m_logo;
	CSpriteObject_2D		m_loading_background;
	int						m_alpha;

public:
	
	CSceneLoading_2D(std::string name);
	
	~CSceneLoading_2D();


	//Sate Change
	virtual void enter(HWND hWnd, HINSTANCE m_hInstance, int nWndClientWidth, int nWndClientHeight);

	virtual void exit();

	virtual void pause();

	virtual void resume();

	//Render & Logic
	virtual void OnDraw(HDC hdc);

	virtual void OnUpdate(float fTimeElapsed);


	//Call
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void SetWindowsSizeChange(int width, int height);

	virtual void CallMenu(WPARAM wParam);


	void ChoPa(HDC hdc)
	{

		//Ææ ¼³Á¤ 
		HPEN hNewPen = CreatePen(PS_SOLID, 5, 0x00000000);
		HPEN hOldPen = (HPEN)SelectObject(hdc, hNewPen);
		//ºê·¯½¬ »ý¼º
		HBRUSH hNewBrush = CreateSolidBrush(0x00ffffff);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		//¸ðÀÚ 0x003403ff
		//¸¶Å© 0x00d0e8fa
		MoveToEx(hdc, 354, 37, NULL);
		LineTo(hdc, 333, 70);
		LineTo(hdc, 365, 87);
		LineTo(hdc, 352, 112);
		LineTo(hdc, 384, 130);
		LineTo(hdc, 406, 101);
		LineTo(hdc, 440, 115);
		LineTo(hdc, 464, 83);
		LineTo(hdc, 428, 65);
		LineTo(hdc, 449, 38);
		LineTo(hdc, 418, 16);
		LineTo(hdc, 394, 54);
		LineTo(hdc, 354, 37);
		MoveToEx(hdc, 156, 128, NULL);
		LineTo(hdc, 167, 145);
		MoveToEx(hdc, 156, 128, NULL);
		LineTo(hdc, 166, 107);
		LineTo(hdc, 199, 61);
		LineTo(hdc, 240, 20);
		LineTo(hdc, 268, 0);
		MoveToEx(hdc, 621, 0, NULL);
		LineTo(hdc, 629, 20);
		LineTo(hdc, 633, 68);
		MoveToEx(hdc, 619, 91, NULL);
		LineTo(hdc, 632, 70);
		LineTo(hdc, 657, 44);
		LineTo(hdc, 678, 29);
		LineTo(hdc, 688, 26);
		LineTo(hdc, 700, 26);
		LineTo(hdc, 703, 39);
		LineTo(hdc, 686, 79);
		LineTo(hdc, 675, 92);
		MoveToEx(hdc, 711, 105, NULL);
		LineTo(hdc, 673, 93);
		LineTo(hdc, 649, 90);
		LineTo(hdc, 626, 89);
		LineTo(hdc, 590, 91);
		LineTo(hdc, 540, 97);
		LineTo(hdc, 481, 111);
		LineTo(hdc, 426, 134);
		LineTo(hdc, 380, 159);
		LineTo(hdc, 332, 191);
		LineTo(hdc, 285, 232);
		LineTo(hdc, 257, 268);
		LineTo(hdc, 238, 296);
		LineTo(hdc, 227, 326);
		LineTo(hdc, 225, 349);
		LineTo(hdc, 231, 379);
		LineTo(hdc, 247, 411);
		LineTo(hdc, 269, 425);
		LineTo(hdc, 332, 438);
		LineTo(hdc, 380, 430);
		MoveToEx(hdc, 250, 414, NULL);
		LineTo(hdc, 256, 393);
		LineTo(hdc, 264, 422);
		MoveToEx(hdc, 256, 393, NULL);
		LineTo(hdc, 269, 353);
		LineTo(hdc, 291, 314);
		LineTo(hdc, 310, 287);
		LineTo(hdc, 341, 258);
		LineTo(hdc, 391, 225);
		LineTo(hdc, 453, 192);
		LineTo(hdc, 510, 177);
		LineTo(hdc, 577, 165);
		LineTo(hdc, 668, 166);
		LineTo(hdc, 691, 171);
		LineTo(hdc, 711, 175); //¸ðÀÚ ¾Æ·¡ ³¡
		MoveToEx(hdc, 711, 299, NULL);
		LineTo(hdc, 694, 320);
		LineTo(hdc, 682, 329);
		//¿ÞÂÊ±Í
		MoveToEx(hdc, 274, 245, NULL);
		LineTo(hdc, 269, 238);
		LineTo(hdc, 251, 236);
		LineTo(hdc, 218, 241);
		LineTo(hdc, 197, 251);
		LineTo(hdc, 177, 261);
		LineTo(hdc, 175, 267);
		LineTo(hdc, 186, 278);
		LineTo(hdc, 202, 287);
		LineTo(hdc, 224, 287);
		LineTo(hdc, 244, 285);
		//¿ÞÂÊ »Ô
		MoveToEx(hdc, 242, 238, NULL);
		LineTo(hdc, 231, 229);
		LineTo(hdc, 224, 221);
		LineTo(hdc, 218, 211);
		LineTo(hdc, 211, 184);
		LineTo(hdc, 199, 167);
		LineTo(hdc, 181, 151);
		LineTo(hdc, 158, 141);
		LineTo(hdc, 137, 138);
		LineTo(hdc, 112, 141);
		LineTo(hdc, 108, 155);//¸®ÅÏ
		LineTo(hdc, 119, 156);
		LineTo(hdc, 137, 160);
		LineTo(hdc, 148, 167);
		MoveToEx(hdc, 108, 155, NULL);
		LineTo(hdc, 101, 166);
		LineTo(hdc, 106, 179);
		LineTo(hdc, 131, 180);
		LineTo(hdc, 158, 189);
		LineTo(hdc, 170, 205);
		LineTo(hdc, 174, 216);
		LineTo(hdc, 169, 230);
		LineTo(hdc, 159, 244);
		LineTo(hdc, 142, 250);
		LineTo(hdc, 130, 248);
		LineTo(hdc, 118, 239);
		LineTo(hdc, 110, 224);
		LineTo(hdc, 105, 212);
		LineTo(hdc, 95, 214);
		LineTo(hdc, 90, 224);//¸®ÅÏ
		LineTo(hdc, 100, 249);
		MoveToEx(hdc, 90, 224, NULL);
		LineTo(hdc, 82, 230);
		LineTo(hdc, 77, 238);
		LineTo(hdc, 87, 255);
		MoveToEx(hdc, 77, 238, NULL);
		LineTo(hdc, 70, 243);
		LineTo(hdc, 69, 253);
		LineTo(hdc, 79, 273);
		LineTo(hdc, 91, 286);
		LineTo(hdc, 113, 298);
		LineTo(hdc, 147, 303);
		LineTo(hdc, 170, 295);
		LineTo(hdc, 191, 283);
		//¿À¸¥ÂÊ »Ô
		MoveToEx(hdc, 632, 56, NULL);
		LineTo(hdc, 644, 41);
		LineTo(hdc, 636, 29);
		LineTo(hdc, 630, 10);
		LineTo(hdc, 628, 0);
		MoveToEx(hdc, 690, 0, NULL);
		LineTo(hdc, 698, 6);
		LineTo(hdc, 711, 5);
		MoveToEx(hdc, 687, 79, NULL);
		LineTo(hdc, 703, 72);
		LineTo(hdc, 711, 65);

		//±Í¾È
		MoveToEx(hdc, 648, 89, NULL);
		LineTo(hdc, 656, 72);
		LineTo(hdc, 660, 70);
		LineTo(hdc, 653, 89);
		MoveToEx(hdc, 258, 265, NULL);
		LineTo(hdc, 235, 262);
		LineTo(hdc, 250, 260);
		LineTo(hdc, 258, 265);

		//¾ó±¼
		MoveToEx(hdc, 333, 307, NULL);
		LineTo(hdc, 347, 286);
		LineTo(hdc, 380, 256);
		LineTo(hdc, 445, 221);
		LineTo(hdc, 490, 203);
		LineTo(hdc, 540, 192);
		LineTo(hdc, 576, 185);
		LineTo(hdc, 623, 187);
		LineTo(hdc, 652, 200);
		LineTo(hdc, 667, 216);
		LineTo(hdc, 678, 236);
		LineTo(hdc, 684, 257);
		LineTo(hdc, 685, 291);
		LineTo(hdc, 681, 328);
		LineTo(hdc, 662, 364);
		LineTo(hdc, 646, 386);
		LineTo(hdc, 613, 411);
		LineTo(hdc, 583, 427);

		MoveToEx(hdc, 333, 307, NULL);
		LineTo(hdc, 331, 321);
		LineTo(hdc, 333, 348);
		LineTo(hdc, 341, 380);
		LineTo(hdc, 356, 406);
		LineTo(hdc, 386, 432);
		LineTo(hdc, 411, 444);
		LineTo(hdc, 456, 450);
		LineTo(hdc, 498, 449);
		LineTo(hdc, 558, 438);
		LineTo(hdc, 567, 434);
		//´«½ç
		MoveToEx(hdc, 530, 234, NULL);
		LineTo(hdc, 543, 217);
		MoveToEx(hdc, 442, 276, NULL);
		LineTo(hdc, 421, 270);
		//´« ±ò
		Ellipse(hdc, 363, 289, 458, 377);
		Ellipse(hdc, 379, 303, 443, 360);
		Ellipse(hdc, 554, 214, 640, 298);
		Ellipse(hdc, 569, 230, 625, 283);
		//ÄÚ
		MoveToEx(hdc, 507, 331, NULL);
		LineTo(hdc, 516, 325);
		LineTo(hdc, 530, 324);
		LineTo(hdc, 541, 328);
		LineTo(hdc, 541, 335);
		LineTo(hdc, 538, 338);
		LineTo(hdc, 529, 346);// ÄÚ³¡
		LineTo(hdc, 516, 348);
		LineTo(hdc, 507, 346);
		LineTo(hdc, 504, 343);
		LineTo(hdc, 504, 337);
		LineTo(hdc, 507, 331);
		//ÀÔ
		MoveToEx(hdc, 529, 346, NULL);
		LineTo(hdc, 530, 355);
		LineTo(hdc, 529, 364);
		LineTo(hdc, 528, 370);
		LineTo(hdc, 524, 375);
		LineTo(hdc, 518, 379);
		LineTo(hdc, 511, 381);
		LineTo(hdc, 502, 382);
		LineTo(hdc, 495, 381);
		LineTo(hdc, 492, 380);
		LineTo(hdc, 486, 376);
		MoveToEx(hdc, 529, 346, NULL);
		LineTo(hdc, 535, 357);
		LineTo(hdc, 542, 361);
		LineTo(hdc, 551, 363);
		LineTo(hdc, 562, 362);
		LineTo(hdc, 570, 359);
		LineTo(hdc, 578, 354);
		LineTo(hdc, 584, 345);
		LineTo(hdc, 583, 339);
		//¿Þ¸ñ
		MoveToEx(hdc, 567, 434, NULL);
		LineTo(hdc, 557, 455);
		LineTo(hdc, 544, 448);
		LineTo(hdc, 536, 450);
		LineTo(hdc, 522, 452);
		LineTo(hdc, 512, 451);
		LineTo(hdc, 506, 454);
		LineTo(hdc, 501, 462);
		LineTo(hdc, 501, 467);
		LineTo(hdc, 520, 465);
		LineTo(hdc, 537, 469);
		LineTo(hdc, 565, 490);
		LineTo(hdc, 591, 522);
		LineTo(hdc, 593, 525);
		LineTo(hdc, 585, 527);
		LineTo(hdc, 569, 539);
		LineTo(hdc, 559, 520);
		LineTo(hdc, 545, 503);
		LineTo(hdc, 511, 485);
		LineTo(hdc, 501, 486);
		LineTo(hdc, 494, 492);
		LineTo(hdc, 492, 485);
		LineTo(hdc, 494, 472);
		LineTo(hdc, 502, 468);

		MoveToEx(hdc, 499, 451, NULL);
		LineTo(hdc, 488, 462);
		LineTo(hdc, 478, 468);
		LineTo(hdc, 466, 479);
		LineTo(hdc, 451, 494);
		LineTo(hdc, 439, 523);
		LineTo(hdc, 432, 559);
		LineTo(hdc, 435, 579);
		LineTo(hdc, 451, 593);
		LineTo(hdc, 450, 552);
		LineTo(hdc, 455, 532);
		LineTo(hdc, 464, 504);
		LineTo(hdc, 478, 479);
		LineTo(hdc, 489, 462);
		//¿ÞÆÈ
		MoveToEx(hdc, 492, 492, NULL);
		LineTo(hdc, 482, 521);
		LineTo(hdc, 477, 549);
		LineTo(hdc, 474, 574);
		LineTo(hdc, 476, 593);
		LineTo(hdc, 482, 606);
		LineTo(hdc, 495, 614);
		LineTo(hdc, 519, 617);
		LineTo(hdc, 535, 616);
		LineTo(hdc, 559, 609);
		LineTo(hdc, 573, 611);
		LineTo(hdc, 596, 604);
		LineTo(hdc, 614, 595);
		LineTo(hdc, 620, 586);
		LineTo(hdc, 596, 557);
		LineTo(hdc, 588, 544);
		LineTo(hdc, 586, 527);//¹ß±Á ½ÃÀÛ
		LineTo(hdc, 599, 524);
		LineTo(hdc, 616, 529);
		LineTo(hdc, 614, 546);
		LineTo(hdc, 632, 555);
		LineTo(hdc, 628, 570);
		LineTo(hdc, 618, 588);
		MoveToEx(hdc, 586, 527, NULL);
		LineTo(hdc, 569, 539);
		LineTo(hdc, 558, 552);
		LineTo(hdc, 555, 559);
		LineTo(hdc, 554, 570);
		LineTo(hdc, 536, 565);
		LineTo(hdc, 526, 564);//ÆÈ ¾ÈÂÊ ½ÃÀÛ
		LineTo(hdc, 501, 570);
		MoveToEx(hdc, 526, 564, NULL);
		LineTo(hdc, 530, 540);
		LineTo(hdc, 535, 523);
		MoveToEx(hdc, 530, 538, NULL);
		LineTo(hdc, 546, 527);
		LineTo(hdc, 547, 516);
		LineTo(hdc, 537, 499);
		MoveToEx(hdc, 451, 589, NULL);
		LineTo(hdc, 462, 599);
		LineTo(hdc, 479, 603);
		MoveToEx(hdc, 451, 589, NULL);
		LineTo(hdc, 451, 612);
		LineTo(hdc, 462, 645);
		LineTo(hdc, 486, 681);
		MoveToEx(hdc, 540, 681, NULL);
		LineTo(hdc, 531, 650);
		LineTo(hdc, 529, 633);
		LineTo(hdc, 529, 617);
		MoveToEx(hdc, 538, 676, NULL);
		LineTo(hdc, 599, 658);
		LineTo(hdc, 650, 644);
		LineTo(hdc, 711, 617);
		MoveToEx(hdc, 711, 631, NULL);
		LineTo(hdc, 633, 657);
		LineTo(hdc, 541, 682);
		MoveToEx(hdc, 559, 456, NULL);
		LineTo(hdc, 569, 458);
		LineTo(hdc, 581, 465);
		LineTo(hdc, 583, 472);
		LineTo(hdc, 594, 472);
		LineTo(hdc, 603, 478);
		LineTo(hdc, 605, 482);
		LineTo(hdc, 611, 486);
		LineTo(hdc, 612, 493);
		LineTo(hdc, 620, 495);
		LineTo(hdc, 622, 501);
		LineTo(hdc, 633, 504);
		LineTo(hdc, 640, 509);
		LineTo(hdc, 636, 502);
		LineTo(hdc, 629, 493);
		LineTo(hdc, 630, 485);
		LineTo(hdc, 618, 470);
		LineTo(hdc, 620, 458);
		LineTo(hdc, 607, 450);
		LineTo(hdc, 605, 441);
		LineTo(hdc, 595, 436);
		LineTo(hdc, 589, 429);
		MoveToEx(hdc, 602, 420, NULL);
		LineTo(hdc, 615, 420);
		LineTo(hdc, 614, 432);
		LineTo(hdc, 631, 441);
		LineTo(hdc, 638, 458);
		LineTo(hdc, 644, 463);
		LineTo(hdc, 642, 471);
		LineTo(hdc, 652, 477);
		LineTo(hdc, 651, 489);
		LineTo(hdc, 656, 498);
		LineTo(hdc, 652, 509);
		LineTo(hdc, 656, 518);
		LineTo(hdc, 655, 532);
		LineTo(hdc, 645, 532);
		LineTo(hdc, 638, 529);
		LineTo(hdc, 629, 531);
		LineTo(hdc, 624, 525);
		LineTo(hdc, 615, 524);
		LineTo(hdc, 612, 517);
		LineTo(hdc, 603, 519);
		LineTo(hdc, 594, 518);
		LineTo(hdc, 588, 517);
		MoveToEx(hdc, 608, 419, NULL);
		LineTo(hdc, 622, 423);
		LineTo(hdc, 644, 435);
		LineTo(hdc, 661, 454);
		LineTo(hdc, 690, 499);
		LineTo(hdc, 711, 537);
		LineTo(hdc, 690, 480);//¿À¸¥ÆÈ ½ÃÀÛ
		LineTo(hdc, 672, 450);
		LineTo(hdc, 647, 426);
		LineTo(hdc, 610, 419);

		MoveToEx(hdc, 652, 429, NULL);
		LineTo(hdc, 696, 433);
		LineTo(hdc, 711, 424);
		MoveToEx(hdc, 690, 480, NULL);
		LineTo(hdc, 702, 478);
		LineTo(hdc, 711, 474);
		hNewBrush = CreateSolidBrush(0x00d0e8fa);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 400, 74, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));

		///////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x003403ff);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 285, 142, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 439, 169, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));
		///////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x002000db);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 295, 356, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 256, 412, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x005ebbf3);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 683, 46, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 215, 276, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 555, 448, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x00184e8d);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 188, 189, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 677, 10, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x0080d3ff);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 502, 295, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x0085d3fe);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 502, 523, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 679, 443, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x00061f57);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 602, 542, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x00000e21);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 659, 549, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 544, 539, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 607, 521, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x00269fe7);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 472, 525, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 475, 621, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x0042c0ee);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 457, 502, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 533, 474, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 658, 445, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x009dccec);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 590, 500, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 703, 625, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x00134501);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 522, 335, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x00031a48);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 599, 255, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 417, 337, 0x00ffffff, FLOODFILLSURFACE);

		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x000b1844);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 699, 669, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));
		//////////////////////////////////////////////
		hNewBrush = CreateSolidBrush(0x0058e8dd);
		hOldBrush = (HBRUSH)SelectObject(hdc, hNewBrush);
		ExtFloodFill(hdc, 111, 558, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 707, 494, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 703, 86, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 680, 392, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 638, 41, 0x00ffffff, FLOODFILLSURFACE);
		ExtFloodFill(hdc, 708, 0, 0x00ffffff, FLOODFILLSURFACE);
		DeleteObject(SelectObject(hdc, hOldBrush));



		//Ææ ÇØÁ¦
		DeleteObject(SelectObject(hdc, hOldPen));
	}
};

