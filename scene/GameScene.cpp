#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{ delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	// for (size_t i = 0; i < _countof(worldTransform_); i++)
	//{
	//	// x,y,z方向のスケーリング設定
	//	worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
	//	// x,y,z軸周りの回転角を設定
	//	worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
	//	// x,y,z軸周りの平行移動を設定
	//	worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};
	//	//ワールドトランスフォーム
	//	worldTransform_[i].Initialize();
	// }

	//キャラクターの大元
	worldTransform_[PartId::Root].Initialize();
	//脊髄
	worldTransform_[PartId::Spine].translation_ = {0, 4.5f, 0};
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();

	//上半身
		//Chest
	worldTransform_[PartId::Chest].Initialize();
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].translation_ = {0, 0, 0};
		//Head
	worldTransform_[PartId::Head].Initialize();
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].translation_ = {0, 4.5f, 0};
		//ArmL
	worldTransform_[PartId::ArmL].Initialize();
	worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmL].translation_ = {4.5f, 0, 0};
		//ArmR
	worldTransform_[PartId::ArmR].Initialize();
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmR].translation_ = {-4.5f, 0, 0};

	//下半身
		//Hip
	worldTransform_[PartId::Hip].Initialize();
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].translation_ = {0, -4.5f, 0};
		//LegL
	worldTransform_[PartId::LegL].Initialize();
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegL].translation_ = {4.5f, -4.5f, 0};
		//LegR
	worldTransform_[PartId::LegR].Initialize();
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].translation_ = {-4.5f, -4.5f, 0};
	

	

	//カメラ視点座標を設定
	viewProjection_.eye = {0, 0, -50};
	//カメラの注視点座標を設定
	viewProjection_.target = {0, 0, 0};
	//カメラ上方向ベクトルを設定（右上45度指定）
	//viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};
	viewProjection_.up = {0.0f, 1.0f, 0.0f};
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMComparisonMixed(10.f);
	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定      手前が見えなくなる
	//viewProjection_.nearZ = 40.0f;
	//ファークリップ距離を設定    奥が見えなくなる
	//viewProjection_.farZ = 53.0f;
	//ビュープロジェクション　　　
	viewProjection_.Initialize();
}

void GameScene::Update()
{
	//視点移動処理
	{
	//	//視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};

	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) 
	//	{
	//		move = {0, 0, kEyeSpeed};
	//	} 
	//	else if (input_->PushKey(DIK_S))
	//	{
	//		move = {0, 0, -kEyeSpeed};
	//	}
	//	//視点移動（ベクトルの加算）
	//	viewProjection_.eye.x += move.x;
	//	viewProjection_.eye.y += move.y;
	//	viewProjection_.eye.z += move.z;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)",
			viewProjection_.eye.x,
			viewProjection_.eye.y, 
			viewProjection_.eye.z);

	}
	//注視点移動処理
	{
	//	//注視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) 
	//	{
	//		move = {-kTargetSpeed, 0, 0};
	//	}
	//	else if (input_->PushKey(DIK_RIGHT))
	//	{
	//		move = {kTargetSpeed, 0, 0};
	//	}
	//	
	//	//注視点移動（ベクトルの加算）
	//	viewProjection_.target.x += move.x;
	//	viewProjection_.target.y += move.y;
	//	viewProjection_.target.z += move.z;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)",
			viewProjection_.target.x,
			viewProjection_.target.y,
		    viewProjection_.target.z);
	}
	//上方向回転処理
	{
	//	//上方向の回転速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE))
	//	{
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, XM_2PI);
	//	}
	//	//上方向ベクトルを計算
	//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)",
			viewProjection_.up.x,
			viewProjection_.up.y,
		    viewProjection_.up.z);

	}
	//Fov変更処理
	{
		////上キーで視野角が広がる
		//if (input_->PushKey(DIK_W)) 
		//{
		//	viewProjection_.fovAngleY += 0.01f;
		//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
		//} 
		////下キーで視野角が狭まる
		//else if (input_->PushKey(DIK_S)) {
		//	viewProjection_.fovAngleY -= 0.01f;
		//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		//}
		////行列の再計算
		//viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));
	}
	//クリップ距離変更処理
	{
		////上下キーでニアクリップ距離を増減
		//if (input_->PushKey(DIK_UP)) 
		//{
		//	viewProjection_.nearZ += 0.1f;
		//} 
		//else if (input_->PushKey(DIK_DOWN)) 
		//{
		//	viewProjection_.nearZ -= 0.1f;
		//}
		////行列の再計算
		//viewProjection_.UpdateMatrix();

		//デバック用表示	
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	}
	//キャラクターの移動ベクトル
	{
		//キャラクターの移動ベクトル
		XMFLOAT3 move = {0, 0, 0};

		//キャラクターの移動速さ
		const float kCharactSpeed = 0.2f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) 
		{
			move = {-kCharactSpeed, 0, 0};
		}
		else if (input_->PushKey(DIK_RIGHT))
		{
			move = {kCharactSpeed, 0, 0};
		}
		//注視点移動(ベクトルの加算)
		worldTransform_[PartId::Root].translation_.x += move.x;
		worldTransform_[PartId::Root].translation_.y += move.y;
		worldTransform_[PartId::Root].translation_.z += move.z;

		//デバック用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
		  "Root:(%f,%f,%f)",
		  worldTransform_[PartId::Root].translation_.x,
		  worldTransform_[PartId::Root].translation_.y,
		  worldTransform_[PartId::Root].translation_.z);

		worldTransform_[PartId::Root].UpdateMatrix();
		worldTransform_[PartId::Spine].UpdateMatrix();
		worldTransform_[PartId::Chest].UpdateMatrix();
		worldTransform_[PartId::Head].UpdateMatrix();
		worldTransform_[PartId::ArmL].UpdateMatrix();
		worldTransform_[PartId::ArmR].UpdateMatrix();
		worldTransform_[PartId::Hip].UpdateMatrix();
		worldTransform_[PartId::LegL].UpdateMatrix();
		worldTransform_[PartId::LegR].UpdateMatrix();

		//親行列の指定がある場合は、掛け算する
		if (parent_) 
		{
			//matWorld_ += parent_->matWorld;
		}

		//上半身回転処理
		{
			//上半身の回転の速さ
			const float kChestRotSpeed = 0.05F;
			//押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_U)) 
			{
				worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
			} else if (input_->PushKey(DIK_I)) 
			{
				worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
			}
		}
		//下半身回転処理
		{
			//下半身の回転の速さ
			const float kHipRotSpeed = 0.05F;
			//押した方向で移動ベクトルを変更
			if (input_->PushKey(DIK_J)) {
				worldTransform_[PartId::Hip].rotation_.y -= kHipRotSpeed;
			} 
			else if (input_->PushKey(DIK_K)) 
			{
				worldTransform_[PartId::Hip].rotation_.y += kHipRotSpeed;
			}
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		//model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
		//model_->Draw(worldTransform_[PartId::Root], viewProjection_, textureHandle_);
		//model_->Draw(worldTransform_[PartId::Spine], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::ArmL], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHandle_);
	}
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}