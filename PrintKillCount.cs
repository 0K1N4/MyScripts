/*=====
<PrintKillCount.cs>
└作成者：takagi

＞内容
討伐数表示の実装

＞注意事項
自動的にTMPを付与するので設定していないと変な位置に出ることにもなりえます。

＞更新履歴
__Y25
_M05
D
30:仮作成(緊急):takagi
_M06
D
14:バトルデータの登場に伴う軽いリファクタリング:takagi
=====*/

// 名前空間宣言
using TMPro;
using UnityEngine;

// クラス定義
[RequireComponent(typeof(TextMeshProUGUI))]
public class CPrintKillCount : MonoBehaviour
{
	// 変数宣言
	[SerializeField, Tooltip("討伐数表示場所")] private TextMeshProUGUI m_TMP;


	/// <summary>
	/// -討伐数表示文取得関数
	/// <para>討伐数出力テキストの共通化を図る処理</para>
	/// </summary>
	/// <returns>討伐数の出力テキスト</returns>
	private string GetKillPrint()
	{
		// 提供	
		return "kill: " + CBattleData.Instance.KillCount;	// 討伐数の表示方法
	}
	
	/// <summary>
	/// -初期化関数
	/// <para>初期化処理</para>
	/// </summary>
	private void Start()
	{
		// テキスト初期化
		m_TMP = GetComponent<TextMeshProUGUI>();	// 機能取得
		m_TMP.text = GetKillPrint();	// 表示内容初期化
	}
	
	/// <summary>
	/// -更新関数	//TODO:イベントで敵が倒されたときだけ呼び出されるように
	/// <para>更新処理</para>
	/// </summary>
	private void Update()
	{
		// 更新
		if (m_TMP != null)	// ヌルチェック
		{
			m_TMP.text = GetKillPrint();	// 表示更新
		}
	}
}
