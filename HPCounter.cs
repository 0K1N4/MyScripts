/*=====
<HPBar.cs>
└作成者：okugami

＞内容
Hpバーの制御スクリプト

＞更新履歴
__Y25
_M07
D
04:作成　okugami
=====*/


using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class HPCounter : MonoBehaviour
{
    public CHitPoint m_cHitPoint;
    public TextMeshProUGUI m_nowHP;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        // 必要な参照が設定されているか確認
        if (m_cHitPoint == null || m_nowHP == null)
        {
            Debug.LogWarning("HPCounter: 必要な参照が設定されていません。");
            return;
        }

        // 初期表示
        UpdateHPText();

        // イベント登録
        m_cHitPoint.OnDamaged += UpdateHPText;
        m_cHitPoint.OnHealed += UpdateHPText;
        m_cHitPoint.OnDead += UpdateHPText;
    }

    private void OnDestroy()
    {
        // イベント解除（メモリリーク防止）
        if (m_cHitPoint != null)
        {
            m_cHitPoint.OnDamaged -= UpdateHPText;
            m_cHitPoint.OnHealed -= UpdateHPText;
            m_cHitPoint.OnDead -= UpdateHPText;
        }
    }

    /// <summary>
    /// HP表示を更新する
    /// </summary>
    private void UpdateHPText()
    {
        m_nowHP.text = m_cHitPoint.HP.ToString();
    }

}
