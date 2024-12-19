# 介紹
這個作品使用了 ESP8266、SSD1306 及 KeyPad 來實現丼字遊戲，並且還實現了人機對戰。
# 接線
- SSD1306 使用 I2C 通訊
- KeyPad
	* Row 接腳為 D0、D3、D4
	* Column 接腳為 D5、D6、D7
	* 使用到的按鍵為數字鍵 1~9
# 操作介面
1. 開機畫面
	- 按下 1 可以進入遊戲
	- 按下 2 可進入設定
	- 按下 9 可以返回
<Img src="https://i.imgur.com/c2rwLmJ.jpeg" width="400px" />

2. 設定畫面
	- 按下 1 進入設定機器人難度<br/>
	- 按下 2 進入設定先後順序（O 先）<br/>
	- 按下 3 進入設定是否使用機器人
	<Img src="https://i.imgur.com/IiF4I9B.jpeg" width="400px" />
3. 機器人難度選擇畫面
	- 按下 1~4 可選擇對應的難度，目前選擇的難度會在前面以 * 號表示<br/>
	- 按下 9 可以返回
<Img src="https://i.imgur.com/T3dlT03.jpeg" width="400px" />

4. 先後順序選擇畫面
	- 按下 1 代表玩家為 O（先手）<br/>
	- 按下 2 代表玩家為 X（後手）<br/>
	- 按下 9 可以返回
<Img src="https://i.imgur.com/FK1IsJJ.jpeg" width="400px" />

5. 是否使用機器人選擇
	- 按下 1 使用機器人<br/>
	- 按下 2 不使用機器人，代表要跟真人對打<br/>
	- 按下 9 可以返回
<Img src="https://i.imgur.com/4WFY5uq.jpeg" width="400px" />

6. 遊玩畫面
	- 數字鍵 1~9 代表九官格的每一格<br/>
	- 按下之後需要再按下同一格代表確定，按下其它格則代表取消
<Img src="https://i.imgur.com/nf37Qtc.jpeg" width="400px" />
