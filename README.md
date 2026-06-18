# 2026 OOPL Final Report

## 組別資訊

組別:第55組
組員：
- 113590026 江柏勳
- 113590027 余政澤

遊戲名稱：Just Shapes & Beats

## 專案簡介

### 遊戲簡介

此專案嘗試復刻《Just Shapes & Beats》，透過移動和衝刺來閃避隨著音樂節奏出現的各種障礙並撐到音樂或關卡結束，其中的障礙不只有四處飛來的尖刺或方塊，也有貫穿畫面的雷射。

### 組別分工

- 余政澤(後端邏輯): 子彈生成繪製系統、碰撞判定系統。
- 江柏勳(前端排版): 畫面及設定系統、圖譜繪製、玩家操作系統。

### Setting Before Building

`config.hpp`:
```c++
constexpr unsigned int WINDOW_WIDTH = 1920;
constexpr unsigned int WINDOW_HEIGHT = 1080;
```

若無法建置，請嘗試對`CMakeLists.txt`右鍵使用`Reload CMake Project`，<br>
或者建置時加入`-DCMAKE_POLICY_VERSION_MINIMUM=3.5`。

請先確保螢幕顯示器比例為`125%`(筆電預設值)再遊玩，以防破圖。

## 遊戲介紹

### 遊戲規則

玩家可使用鍵盤或滑鼠於關卡選擇指定關卡，進去關卡後則只能使用鍵盤來進行操控。

可用按鍵如下

| 按鍵 | 動作 |
| -------- | -------- |
| W | 向上移動     |
| S | 向下移動     |
| A | 向左移動     |
| D | 向右移動     |
| Space | 衝刺    |
| Enter | 選定    |
| Mouse LeftClick | 選定/選擇    |
| Esc | 返回或開啟設定(於主畫面則是關閉遊戲)     |
| Tab | 開啟作弊模式    |
| O | 開啟設定(遊玩關卡時為esc)     |
| 方向鍵(↑) | 向上移動     |
| 方向鍵(↓) | 向下移動     |
| 方向鍵(←) | 向左移動     |
| 方向鍵(→) | 向右移動     |

根據每首歌的特點和BPM，地圖也會有所不同，子彈部分固定、部分隨機，碰到子彈後會扣除一滴血直至沒有則觸發死亡。

每關都會有特定的CheckLine和玩家血量，碰到CheckLine後會記錄當前時間點，並於玩家死亡後回溯到紀錄點。當觸碰到最後一個檢查點則視為完成關卡，會清空場上所有子彈並回到主畫面。

### 遊戲畫面
本專案包含主畫面、關卡選擇畫面、遊戲畫面、設定畫面。

1. 主畫面
   點擊或使用按鍵選擇「歌曲清單」可進入「關卡選擇畫面」，也可按"O"進入「設定畫面」
   ![螢幕擷取畫面 2026-06-18 205929](https://hackmd.io/_uploads/ry_poPWGMg.png)

2. 關卡選擇畫面
   點擊或使用方向鍵選擇歌曲後按下"Enter"或再次點擊可進入「遊戲畫面」並遊玩所選歌曲
   ![螢幕擷取畫面 2026-06-18 210017](https://hackmd.io/_uploads/ByVe2vZzGx.png)

3. 遊戲畫面
   背景會開始播放音樂，並跟隨著節拍召喚子彈，受擊時會有短暫無敵效果
   ![螢幕擷取畫面 2026-06-18 210145](https://hackmd.io/_uploads/ryiSnvZMMe.png)

   畫面中藍色虛線則為CheckLine，紀錄遊玩時抵達的時間點。
   ![螢幕擷取畫面 2026-06-18 210811](https://hackmd.io/_uploads/HJeCaPZffe.png)


4. 設定畫面
   於遊戲中可回到主畫面或重新遊玩，預設包含音樂和音效控制。
   ![螢幕擷取畫面 2026-06-18 210054](https://hackmd.io/_uploads/S1ZXhwWfMe.png)


## 程式設計

### 程式架構

**繼承關係**

- Screen (介面)
  - MainMenuScreen
  - SongListScreen
  - PlaygroundScreen
  - SettingScreen
- GameObject
  - EventObject (模板)
    - Button
    - MergeButton (複合物件)
    - SongListItem (複合物件)
    - DiscreteSlider (複合物件)
  - ImageObject
  - TextObject
  - Player
  - FadeLayer
  - LevelSpawner
    -Drawable
  - CustomColorShape
  - BatchedColorShape
  - BatchedCircleShape

**組合關係**

- SongList (音樂資料庫)
  - SongData (音樂資料格式)
  - SongsBPM
- App
  - Screen
  - SettingScreen
  - MusicPlayerManager
- All Screens
  - ...(畫面上的物件)
- LevelSpawner
  - Obstacle
  - BatchedColorShape
  - SpawnEvent

**獨立物件**

- MusicPlayerManager
- SongList
  - SongData

**類型、結構**

- Levels
- ScreenState
- SongData (歌曲資料架構)
  - SongsBPM
- BulletType

### 程式技術

1. 物件池 : 預先生成20000個空Obstacle，有生成需求時再提供對應資料(SpawnEvent)進行生成，避免在某一拍要求過多子彈時造成卡頓。
2. 物件轉型(Object Casting) : 在畫面中的物件觸發Foucus時，通常會有一個NowSelect的父類指向這個子類物件。
3. 批次渲染子彈(BatchedColorShape) : BeginBatch() 與 EndBatch() 收集渲染數據並確定這幀的數據，將大量存活彈幕的頂點、UV 與顏色資料打包，一次性推入緩衝區發送給 GPU，大幅降低 Draw Call 以維持幀率。
4. 組合 : 一個Screen通常由多個物件所構成。
5. 介面(Interface) : Screen本身不包括任何實際作為。
6. 繼承 : 大多數物件都由GameObject繼承而來。
7. 資料驅動與 Lambda : 在LevelSpawner::CreateObstacle，摒棄為每種彈幕建立子類別的做法，將軌跡、變形邏輯寫成 Lambda 函式，依賴當前節拍與玩家座標進行即時運算。
8. 多型 : 在如TextObject的建構子中，因為預設值的緣故，參數數量的不同可能改變建構子的行為。
9. 依賴注入 : SongList中的功能即給予他物SongData內部資料的功能。
10. 工廠模式(Factory Pattern) : LevelSpawner透過SpawnEvent生成此歌曲需要的子彈名單並預生成物件池避免卡頓，再透過Update根據當前節拍
- 呼叫GetActiveObstacle()，抓出一顆 IsActive == false 的閒置子彈。
- 抓取SpawnEvent.top()將各種參數傳給Obstacle並呼叫Obstacle.Spawn()進行新參數的匯入。

- 最後再去輪詢物件池進行Update()。

### AI/AI Agent使用

1. Gemini
2. Copilot
3. NotebookLM

輔助於初始理解畫圖機制、畫面架構、PTSD內部底層機制、功能與架構。

一開始用於協助釐清該如何生成子彈。因為物件數很多，所以在AI的協助下完成如何批次繪製各種子彈這課題。像是LevelSpawner和BatchedColorShape就是先請AI完成雛形，再由我們調整和修正成我們所需要的功能和架構。

部分邏輯上的優化也是參考AI提供的想法或程式碼去進行修正。


## 結語



### 問題與解決方法

1. 即將完成第二個關卡時突然遇到子彈卡頓問題，而且發生時間點是在最後幾種子彈完成後發生的。一開始為了改顏色會在行為邏輯那宣告Uvs，導致每次Update就宣告一次，少量的子彈不影響，但那次剛好會一口氣生成上千個，就導致了卡頓。

   解決方法 : 在Obstacle宣告一個Uvs用來暫存變化後的Uvs，以解決每次都重新宣告。

2. SDL內建工具無法抓取音樂撥放時長。

   解決方法 : 引入第三方工具SoLoud。

3. 物件觸發前的狀態被觸發後覆蓋。

   解決方法 : 把兩種狀態分開寫、分開存

4. 音樂時間跳轉異常。

   解決方法 : 把造成異常的末數位小數去除

5. 方塊有機率被旋轉90度

   解決方法 : 因為那種子彈不需旋轉，所以並沒有在SpawnEvent賦值，導致在生成時抓取了未知的值，所以即使不需旋轉也須給個0避免出問題。

### 自評

- [x] 完成專案權限改為 public
- [x] 具有 debug mode 的功能
- [x] 解決專案上所有 Memory Leak 的問題
- [x] 報告中沒有任何錯字，以及沒有任何一項遺漏
- [x] 報告至少保持基本的美感，人類可讀

### 心得

這次的實習學到了多少物件導向的概念我不確定，但是讓我學到了從0開發遊戲是多麼麻煩且困難的一件事。經過了這次的經歷，讓我深刻了解了遊戲引擎的美好，哪怕複雜、難懂，但符合人體工學[(?)](https://zh.wikipedia.org/zh-tw/%E4%BA%BA%E5%9B%A0%E5%B7%A5%E7%A8%8B%E5%AD%A6)，就是好工具。

最後我想說PTSD是個好框架，好用多用下次還用。~~<font size="1">做PTSD得PTSD</font>~~

### 貢獻比例

- 余政澤:50%
- 江柏勳:50%

(由於第三方模組的引入，github的貢獻僅供參考，其原因是json和soloud)


