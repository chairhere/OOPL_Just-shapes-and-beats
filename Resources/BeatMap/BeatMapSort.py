import json

def sort_rhythm_game_data(file_path):
    try:
        # 1. 讀取原始資料
        with open(file_path, 'r', encoding='utf-8') as f:
            data = json.load(f)

        # 2. 進行排序：根據 'StartBeat' 從小到大排序
        # x.get('StartBeat', 0) 確保如果沒抓到值，會排在最前面
        data.sort(key=lambda x: x.get('StartBeat', 0))

        # 3. 寫回原本的檔案 (或是另存新檔)
        output_path = file_path.replace('.json', '_sorted.json')
        with open(output_path, 'w', encoding='utf-8') as f:
            # indent=2 會讓格式長得像你範例中的樣子（縮進兩格）
            json.dump(data, f, indent=2, ensure_ascii=False)

        print(f"排序完成！已儲存至: {output_path}")

    except Exception as e:
        print(f"處理失敗: {e}")

if __name__ == "__main__":
    # 請確保你的檔案放在同一個資料夾下，並將檔名改為你的檔名
    sort_rhythm_game_data('Milky Way.json')