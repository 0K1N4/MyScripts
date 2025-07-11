using TMPro;
using UnityEngine;
using UnityEngine.UI;


public class KillCount : MonoBehaviour
{
    public TextMeshProUGUI m_nowKill;

    void OnEnable()
    {
        CBattleData.Instance.OnKillCountChanged += UpdateKillText;
        UpdateKillText(CBattleData.Instance.KillCount); // èâä˙ï\é¶
    }

    void OnDisable()
    {
        CBattleData.Instance.OnKillCountChanged -= UpdateKillText;
    }

    void UpdateKillText(uint newKillCount)
    {
        m_nowKill.text = newKillCount.ToString("F0");
    }

}

