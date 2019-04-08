# �����
import jieba.analyse  # ����ؼ�����ȡ��
import pandas as pd  # ����pandas
import newspaper
# ��ȡ�ı�����
# ��ȡ���� ��������̨����Ϊ��
article = newspaper.Article('https://finance.sina.com.cn/money/bank/bank_hydt/2019-02-25/doc-ihsxncvf7656807.shtml', language='zh')
# ��������
article.download()
# ��������
article.parse()
# �����½���nlp����
article.nlp()
# nlp����������ƴ��
string_data = "".join(article.keywords)
# �ؼ�����ȡ
def get_key_words(string_data, how=''):
    # topK����ȡ�Ĺؼ�����������ָ������ȡȫ����
    # withWeight������ΪTrueָ������ʶ�Ӧ��IF-IDFȨ��
    if how == 'textrank':
        # ʹ��TextRank �㷨
        tags_pairs = jieba.analyse.textrank(string_data,  topK=5,  withWeight=True)  # ��ȡ�ؼ��ֱ�ǩ
    else:
        # ʹ��TF-IDF �㷨
        tags_pairs = jieba.analyse.extract_tags(string_data,  topK=5,  withWeight=True)  # ��ȡ�ؼ��ֱ�ǩ
    tags_list = []  # ���б������洢��ֺ������ֵ
    for i in tags_pairs:  # ��ӡ��ǩ�������TF-IDFȨ��
        tags_list.append((i[0], i[1]))  # ��������ֶ�ֵ
    tags_pd = pd.DataFrame(tags_list, columns=['word', 'weight'])  # �������ݿ�
    return tags_pd

keywords = get_key_words(string_data)
print("#####################TF-IDF####################")
print(keywords)

keywords_tr = get_key_words(string_data, how='textrank')
print("#####################textrank####################")
print(keywords_tr)
