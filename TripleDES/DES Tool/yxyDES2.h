/*----------------------------------------------------------------
            // Copyright (C) 2008 ��ѧԨ
            // ��Ȩ���С�
            //
            // �ļ�����yxyDES2.h
            // �ļ�����������DES2������ ͷ�ļ�
            //
            //
            // �����ˣ���ѧԨ
            //
            // �޸��ˣ�
            // �޸�������
            //
            // �޸��ˣ�
            // �޸�������
//----------------------------------------------------------------*/
//---------------------------------------------------------------------------

#ifndef yxyDESH
#define yxyDESH

#include <string>

class yxyDES2
{
public:
		//�๹�캯��
         yxyDES2(); 

		 //����������
        ~yxyDES2(); 

        //����:����16��28λ��key
        //����:Դ8λ���ַ���(key),���key�����0-1
        //���:����������private CreateSubKey���������char SubKeys[keyN][16][48]
        void InitializeKey(byte* srcBytes,unsigned int keyN);

        //����:����8λ�ַ���
        //����:8λ�ַ���,ʹ��Key�����0-1
        //���:���������ܺ��������private szCiphertext[16]
        //      �û�ͨ������Ciphertext�õ�
        void EncryptData(byte* _srcBytes,unsigned int keyN);

        //����:����16λʮ�������ַ���
        //����:16λʮ�������ַ���,ʹ��Key�����0-1
        //���:���������ܺ��������private szPlaintext[8]
        //      �û�ͨ������Plaintext�õ�
        void DecryptData(byte* _srcBytes,unsigned int keyN);

        //����:�������ⳤ���ַ���
        //����:���ⳤ���ַ���,����,ʹ��Key�����0-1
        //���:���������ܺ��������private szFCiphertextAnyLength[8192]
        //      �û�ͨ������CiphertextAnyLength�õ�
        void EncryptAnyLength(byte* _srcBytes,unsigned int _bytesLength,unsigned int keyN);

        //����:�������ⳤ��ʮ�������ַ���
        //����:���ⳤ���ַ���,����,ʹ��Key�����0-1
        //���:���������ܺ��������private szFPlaintextAnyLength[8192]
        //      �û�ͨ������PlaintextAnyLength�õ�
        void DecryptAnyLength(byte* _srcBytes,unsigned int _bytesLength, unsigned int keyN);

		//����:Bytes��Bits��ת��,
        //����:���任�ַ���,���������Ż�����ָ��,Bits��������С
		void Bytes2Bits(byte *srcBytes, byte* dstBits, unsigned int sizeBits);

		//����:Bits��Bytes��ת��,
        //����:���任�ַ���,���������Ż�����ָ��,Bits��������С
		void Bits2Bytes(byte *dstBytes, byte* srcBits, unsigned int sizeBits);

		//����:Int��Bits��ת��,
        //����:���任�ַ���,���������Ż�����ָ��
		void Int2Bits(unsigned int srcByte, byte* dstBits);
		
		//����:Bits��Hex��ת��
        //����:���任�ַ���,���������Ż�����ָ��,Bits��������С
		void Bits2Hex(byte *dstHex, byte* srcBits, unsigned int sizeBits);
		
		//����:Bits��Hex��ת��
        //����:���任�ַ���,���������Ż�����ָ��,Bits��������С
		void Hex2Bits(byte *srcHex, byte* dstBits, unsigned int sizeBits);

		//szCiphertextInBinary��get����
        byte* GetCiphertextInBinary();

		//szCiphertextInHex��get����
		byte* GetCiphertextInHex();

        //Ciphertext��get����
		byte* GetCiphertextInBytes();

		//Plaintext��get����
        byte* GetPlaintext();

        //CiphertextAnyLength��get����
		byte* GetCiphertextAnyLength();

		//PlaintextAnyLength��get����
        byte* GetPlaintextAnyLength();

private:
        byte szSubKeys[2][16][48];//����2��16��48λ��Կ,��2������3DES
		byte szCiphertextRaw[64]; //�������������(64��Bits) int 0,1
		byte szPlaintextRaw[64]; //�������������(64��Bits) int 0,1
		byte szCiphertextInBytes[8];//����8λ����
        byte szPlaintextInBytes[8];//����8λ�����ַ���

		byte szCiphertextInBinary[65]; //�������������(64��Bits) char '0','1',���һλ��'\0'
		byte szCiphertextInHex[17]; //����ʮ����������,���һλ��'\0'
		byte szPlaintext[9];//����8λ�����ַ���,���һλ��'\0'

        byte szFCiphertextAnyLength[8192];//���ⳤ������
        byte szFPlaintextAnyLength[8192];//���ⳤ�������ַ���

		//����:��������Կ
        //����:����PC1�任��56λ�������ַ���,���ɵ�szSubKeys���0-1
        //���:��������byte szSubKeys[16][48]
        void CreateSubKey(byte* sz_56key,unsigned int keyN);

		//����:DES�е�F����,
        //����:��32λ,��32λ,key���(0-15),ʹ�õ�szSubKeys���0-1
        //���:���ڱ任����32λ
        void FunctionF(byte* sz_Li,byte* sz_Ri,unsigned int iKey,unsigned int keyN);

		//����:IP�任
        //����:�������ַ���,����������ָ��
        //���:�����ı�ڶ�������������
        void InitialPermuteData(byte* _src,byte* _dst);

		//����:����32λ������չλ48λ,
        //����:ԭ32λ�ַ���,��չ�������ָ��
        //���:�����ı�ڶ�������������
        void ExpansionR(byte* _src,byte* _dst);

		//����:�����,
        //����:�����Ĳ����ַ���1,�ַ���2,����������,����������ָ��
        //���: �����ı���ĸ�����������
        void XOR(byte* szParam1,byte* szParam2, unsigned int uiParamLength, byte* szReturnValueBuffer);

		//����:S-BOX , ����ѹ��,
        //����:48λ�������ַ���,
        //���:���ؽ��:32λ�ַ���
        void CompressFuncS(byte* _src48, byte* _dst32);

		//����:IP��任,
        //����:���任�ַ���,����������ָ��
        //���:�����ı�ڶ�������������
        void PermutationP(byte* _src,byte* _dst);

};

#endif
 