/*=====
<HPBar.cs>
└作成者：okugami

＞内容
Hpバーの制御スクリプト

＞更新履歴
__Y25
_M05
D
21:作成　okugami
29:臨時的な修正:takagi
=====*/

using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class HPBar : MonoBehaviour
{
	public Slider m_HpBar;
	public CHitPoint m_cHitPoint;
	//[SerializeField, Tooltip("HP残量表示")] private TextMeshProUGUI m_NowHPText;
	[SerializeField, Tooltip("最大HP表示")] private TextMeshProUGUI m_MaxHPText;

	// Start is called once before the first execution of Update after the MonoBehaviour is created
	void Start()
	{
		
	}

	// Update is called once per frame
	void Update()
	{
		if (m_cHitPoint != null)
		{
			//if (m_NowHPText != null)
			//{
			//	m_NowHPText.text = m_cHitPoint.HP.ToString();
			//}
			if (m_MaxHPText != null)
			{
			m_MaxHPText.text = m_cHitPoint.MaxHP.ToString();
			}

			m_HpBar.value = m_cHitPoint.HP / (float)m_cHitPoint.MaxHP;
		}
	}
}
