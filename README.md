PuzzDraLibs
===========

パズドラのルート解析アルゴリズム研究用プログラム  
PuzzDraSolverリポジトリとPuzzDraVisualizerリポジトリはこちらにまとめられました。

ソースコードと研究レポートの対応  
---  
3.(4)  
以下はすべてPuzzDraSolver/PuzzDraOperator.h内  
1.ドロップの移動     - slideDrop_4関数  
2.コンボ判定         - evalCombo関数  
3.ドロップの押し下げ - pushDownDrops関数  


\4.  
PuzzDraSolver/PuzzDraRoute.h内  
ルートを保持するデータ構造 - Routeクラス  


\5.  
PuzzDraSolver内  
(2)探索手法1 - PuzzDraFirstSearch.cpp内firstSearch_4関数  
(3)探索手法2 - PuzzDraBeamSerach.cpp内beamSearch関数  
(5).4        - PuzzDraBeamSerach.cpp内doubleLimitedSearch関数  


視覚化ソフトウェア - PuzzDraVisualizer
