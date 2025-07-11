using System;
using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class CFadeManager : MonoBehaviour
{
	public string   scene;                    //�J�ڐ�̃V�[����
	public float    Speed;                    //�t�F�[�h����X�s�[�h
	public bool     fadeOnStart = true;       //�Q�[���J�n���Ƀt�F�[�h�C�����邩�ǂ���
	public Image    fadeImage;                //�t�F�[�h�p��Image�R���|�[�l���g
	float           red, green, blue, alfa; 
	Coroutine       fadeCoroutine;

	public static CFadeManager Instance { get; private set; }

	void Awake()
	{
		if (Instance == null) Instance = this;
		else Destroy(gameObject); // �����h�~
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
		if(fadeCoroutine == null)	// �t�F�[�h���łȂ�
		{
			fadeCoroutine = StartCoroutine(FadeOut());	// �t�F�[�h�C���J�n
		}
		//if (fadeCoroutine != null)
		//{
		//	StopCoroutine(fadeCoroutine);
		//}
		//fadeCoroutine = StartCoroutine(FadeOut());
	}

	public void CalledStartFadeOut(string CalledScene)
	{
		scene = CalledScene; // �J�ڐ�̃V�[�������X�V
		if(fadeCoroutine == null)	// �t�F�[�h���łȂ�
		{
			fadeCoroutine = StartCoroutine(FadeOut());	// �t�F�[�h�A�E�g�J�n
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
		
		Debug.Log(" �V�[�� " + scene + " ��ǂݍ��݊J�n");
		if(!string.IsNullOrEmpty(scene))
		{
			SceneManager.LoadScene(scene);
		}
	}
}
