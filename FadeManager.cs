using System;
using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class CFadeManager : MonoBehaviour
{
	public string   scene;                    //遷移先のシーン名
	public float    Speed;                    //フェードするスピード
	public bool     fadeOnStart = true;       //ゲーム開始時にフェードインするかどうか
	public Image    fadeImage;                //フェード用のImageコンポーネント
	float           red, green, blue, alfa; 
	Coroutine       fadeCoroutine;

	public static CFadeManager Instance { get; private set; }

	void Awake()
	{
		if (Instance == null) Instance = this;
		else Destroy(gameObject); // 複数防止
	}

	void Start()
	{
		if(fadeOnStart)
		{
			red = 1f;
			green = 1f;
			blue = 1f;
			alfa = 1f;
		}
		else 
		{
			red = fadeImage.color.r;
			green = fadeImage.color.g;
			blue = fadeImage.color.b;
			alfa = fadeImage.color.a;
		}

		fadeImage.color = new Color(red, green, blue, alfa);
	}

	public void StartFadeIn()
	{
		if (fadeCoroutine != null)
		{
			StopCoroutine(fadeCoroutine);
		}
		fadeCoroutine = StartCoroutine(FadeIn());
	}

	public void StartFadeOut()
	{
		if(fadeCoroutine == null)	// フェード中でない
		{
			fadeCoroutine = StartCoroutine(FadeOut());	// フェードイン開始
		}
		//if (fadeCoroutine != null)
		//{
		//	StopCoroutine(fadeCoroutine);
		//}
		//fadeCoroutine = StartCoroutine(FadeOut());
	}

	public void CalledStartFadeOut(string CalledScene)
	{
		scene = CalledScene; // 遷移先のシーン名を更新
		if(fadeCoroutine == null)	// フェード中でない
		{
			fadeCoroutine = StartCoroutine(FadeOut());	// フェードアウト開始
		}
		if (fadeCoroutine != null)
		{
			StopCoroutine(fadeCoroutine);
			fadeCoroutine = StartCoroutine(FadeOut());
		}
	}

	IEnumerator FadeIn()
	{
		fadeImage.enabled = true;
		alfa = 1f;
		while (alfa > 0f)
		{
			alfa -= Speed * Time.deltaTime;
			fadeImage.color = new Color(red, green, blue, alfa);
			yield return null;
		}
		fadeImage.enabled = false;
	}

	IEnumerator FadeOut()
	{
		fadeImage.enabled = true;
		alfa = 0f;
		while (alfa < 1f)
		{
			alfa += Speed * Time.deltaTime;
			fadeImage.color = new Color(red, green, blue, Mathf.Clamp01(alfa));
			yield return null;
		}
		
		Debug.Log(" シーン " + scene + " を読み込み開始");
		if(!string.IsNullOrEmpty(scene))
		{
			SceneManager.LoadScene(scene);
		}
	}
}
