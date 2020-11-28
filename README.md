# FallChallenge2020

## タスク一覧

### ユーティリティ系の改善

- [x] メモリプールの未使用領域の番号を固定長のキューで管理するようにする
  Stateを格納する生配列とは別に、使用済み（解放済みでない）のポインタを格納する配列を作成する  
  配列はキューとして管理を行う
  ~~対比効果が薄いため却下~~
  TLE対策として実施する

- [x] XorShift版の乱数生成クラスを作成する
  - シード値
    2463534242
  - uint32_t型の乱数値からfloat型の[0, 1.0)を生成する
    next
    nextDouble : (double)((next >> 12) | 0b0'01111111111'0000000000000000000000000000000000000000000000000000) - 1.0f
    nextFloat : (float)((next >> 9) | 0b0'01111111'00000000000000000000000) - 1.0f

- [ ] べき乗配列のコンパイル時定数化
  GeometricSequence<>

### 探査系の改善

- [ ] ZobristHashによる同一盤面の排除

### 評価系の改善

- [x] 各Tierの所有個数の係数評価
- [ ] インベントリの使用状態の評価
- [ ] 次のCast可能パターン数の評価

### 参考

- [ツカモさん(tsukammoの収穫記)](https://tsukammo.hatenablog.com/)
- [いちょうさん(ichyo::blog)](https://blog.ichyo.jp/posts/codinggame-fall-challenge-2020/)
- [ヴァルさん(ヴァルのゲームAI開発記)](https://valgrowth.hatenablog.com/entry/2020/11/24/211900)
- [scolさん(スコルの知恵袋)](https://scol.hatenablog.com/entry/2020/11/24/192948)
- [omuさん(o)](https://omuric.github.io/posts/codingame-fall-challenge-2020/)
- [あっとさん(あっとのTECH LOG)](https://at274.hatenablog.com/entry/2020/11/24/222137)
- [ボンドさん(接着剤の精進日記)](https://bondo.hateblo.jp/entry/2020/11/23/210134)
- [アルメリアさん(ARMERIA)](https://betrue12.hateblo.jp/entry/2020/11/23/180246)
- [iwashi31さん(iwashi31’s diary)](https://iwashi31.hatenablog.com/entry/2020/11/23/180658)
- [くれそんさん(くれそんと競プロ)](https://qlethon.hatenablog.com/entry/2020/11/23/201723)
- [phocomさん(others)](https://phocom.github.io/others/cgfall2020.html)
- [olpheさん(olpheの競プロ帖)](https://olphe.hatenablog.com/entry/2020/11/23/180437)
- [hotaruさん(hotaruの蛍雪日記)](https://hotarunx.hatenablog.com/entry/codingame_fall_challenge_2020)
- [いなにわさん(inani_waonの日記)](https://inaniwa.hatenablog.com/entry/2020/11/23/221154)
- [nafukaさん(Nafuka Lines)](https://nafuka.hatenablog.com/entry/2020/11/23/232147)
- [なーぶさん(Nerveメモ)](https://nrvft.hatenablog.com/entry/2020/11/23/222407)
- [bbgeさん(bbge's garbage)](https://bbge.hateblo.jp/entry/2020/11/23/221502)
- [やまけーさん(yamake's blog)](https://yamakeeee.hatenadiary.com/entry/2020/11/24/210817)
- [ととりさん(ととりがにゃあをする)](https://totori.hatenadiary.com/entry/2020/11/23/183601)
- [ろいさん(Roy_Rの競プロ日記)](https://roy-r.hatenablog.com/entry/2020/11/24/125957)
- [煎茶さん(socha77’s blog)](https://socha77.hatenablog.com/entry/2020/11/24/005226)

- [pb4さん](https://github.com/pb4git/Fall-Challenge-2020/blob/main/README.md)
