/*=====
<HPBar.cs>
���쐬�ҁFokugami

�����e
Hp�o�[�̐���X�N���v�g

���X�V����
__Y25
_M07
D
04:�쐬�@okugami
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
        // �K�v�ȎQ�Ƃ��ݒ肳��Ă��邩�m�F
        if (m_cHitPoint == null || m_nowHP == null)
        {
            Debug.LogWarning("HPCounter: �K�v�ȎQ�Ƃ��ݒ肳��Ă��܂���B");
            return;
        }

        // �����\��
        UpdateHPText();

        // �C�x���g�o�^
        m_cHitPoint.OnDamaged += UpdateHPText;
        m_cHitPoint.OnHealed += UpdateHPText;
        m_cHitPoint.OnDead += UpdateHPText;
    }

    private void OnDestroy()
    {
        // �C�x���g�����i���������[�N�h�~�j
        if (m_cHitPoint != null)
        {
            m_cHitPoint.OnDamaged -= UpdateHPText;
            m_cHitPoint.OnHealed -= UpdateHPText;
            m_cHitPoint.OnDead -= UpdateHPText;
        }
    }

    /// <summary>
    /// HP�\�����X�V����
    /// </summary>
    private void UpdateHPText()
    {
        m_nowHP.text = m_cHitPoint.HP.ToString();
    }

}
