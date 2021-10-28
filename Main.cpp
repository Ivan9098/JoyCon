
# include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include"JoyCon.h"

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));

	// 大きさ 60 のフォントを用意
	const Font font(60);

	// 猫のテクスチャを用意
	const Texture cat(Emoji(U"🐈"));

	// 猫の座標
	Vec2 catPos(640, 450);

	gyro::JoyCon joy = gyro::JoyCon(0);

	Point pt = Cursor::Pos();

	int8 isL;

	while (System::Update())
	{
		
		// テキストを画面の中心に描く
		font(U"Hello, Siv3D!🐣").drawAt(Scene::Center(), Palette::Black);

		// 大きさをアニメーションさせて猫を表示する
		cat.resized(100 + Periodic::Sine0_1(1s) * 20).drawAt(catPos);

		if (joy.buttonStick)pt = Scene::Center();

		if (joy.joyconType == gyro::JoyCon::JoyConType::JoyConL)isL = 1;
		else isL = -1;
		pt.x += joy.gyroZ * 0.015 * 800 / 45*(-isL);
		pt.y += joy.gyroY * 0.015 * 600 / 30*(isL);


		// マウスカーソルに追従する半透明の赤い円を描く
		Circle(pt, 40).draw(ColorF(1, 0, 0, 0.5));

	}
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D Slack (ユーザコミュニティ) への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
