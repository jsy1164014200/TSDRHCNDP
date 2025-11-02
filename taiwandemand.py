# https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&tenderStatusType=%E6%B1%BA%E6%A8%99&sortCol=AWARD_NOTICE_DATE&timeRange=114&pageSize=100

# https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?sortCol=AWARD_NOTICE_DATE&d-3611040-p=2&pageSize=100&tenderStatusType=%E6%B1%BA%E6%A8%99&querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&timeRange=114

# https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?sortCol=AWARD_NOTICE_DATE&querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&d-3611040-p=72&pageSize=100&tenderStatusType=%E6%B1%BA%E6%A8%99&timeRange=114

from sys import orig_argv
import time
import re 
# import random

from bs4 import BeautifulSoup
import requests
  

CLIENT_HEADERS_POOL = [
    # Chrome浏览器（Windows）
    {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
        "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        "Accept-Encoding": "gzip, deflate, br",
        "DNT": "1",
        "Connection": "keep-alive"
    },
    # Firefox浏览器（macOS）
    {
        "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 14_0) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Firefox/118.0",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
        "Accept-Language": "zh-TW,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        "Accept-Encoding": "gzip, deflate, br",
        "Connection": "keep-alive"
    },
    # Safari浏览器（iOS）
    {
        "User-Agent": "Mozilla/5.0 (iPhone; CPU iPhone OS 16_6 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.6 Mobile/15E148 Safari/604.1",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
        "Accept-Language": "zh-CN,zh-Hans;q=0.9",
        "Accept-Encoding": "gzip, deflate, br",
        "Connection": "keep-alive"
    },
    # Edge浏览器（Windows）
    {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36 Edg/118.0.2088.61",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
        "Accept-Language": "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7",
        "Accept-Encoding": "gzip, deflate, br",
        "Connection": "keep-alive"
    },
    # Android Chrome
    {
        "User-Agent": "Mozilla/5.0 (Linux; Android 13; SM-G998B) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Mobile Safari/537.36",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
        "Accept-Language": "zh-CN,zh;q=0.9",
        "Accept-Encoding": "gzip, deflate, br",
        "Connection": "keep-alive"
    }
]



def get_url_data_with_requests(url):
    try:
        # headers = {
        #     "Host": "https://web.pcc.gov.tw",
        #     "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
        #     "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
        #     "Accept-Language": "zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3",
        #     "Accept-Encoding": "gzip, deflate, br",
        #     "Referer": "https://www.example.com",  # 表示从哪个页面跳转过来
        #     "Connection": "keep-alive",
        #     "Upgrade-Insecure-Requests": "1"
        # }
        # cookies = {
        #     "JSESSIONID": "px7C8PDKdwdWCEb224rNrz3DQHSmcB6fO0nEsbwE.aplapp2:instA",
        #     "searchToken": "2Km7fS9PSwpaiVNleLiD",
        #     "cookiesession1":"678ADA6A9FF322A91C23C33D67098734",
        #     "webpcc": "4bb3a3d8ab2edb41738dd541bfdf20a95d2399dd3a852e6c96fa3768646686b388649b39",
        # }
        # headers = random.choice(CLIENT_HEADERS_POOL)
        # 发送GET请求
        response = requests.get(url, timeout=20) # , headers=headers, cookies=cookies)    
        response.raise_for_status() 
        return {
            "status": "success",
            "data": response.text,  # 文本内容（如HTML、JSON字符串）
            # "data": response.content  # 二进制内容（如图片、文件）
        }
    except requests.exceptions.RequestException as e:
        return {
            "status": "error",
            "data": f"请求失败: {str(e)}"
        }

def extract_numbers(text):
    # 正则表达式模式：匹配 d-3611040-p= 后面的数字
    pattern = r'd-3611040-p=(\d+)'
    
    # 查找所有匹配的数字
    numbers = re.findall(pattern, text)
    
    # 转换为整数（如果需要字符串形式可省略此步）
    numbers = [int(num) for num in numbers]
    
    return numbers

def extract_href_content(text):
    # 正则表达式模式：匹配 <a href="..."> 中的内容
    # .*? 表示非贪婪匹配任意字符（除换行符外）
    pattern = r'<a href="/prkms/urlSelector/common/atm?(.*?)">'
    
    # 查找所有匹配的内容
    results = re.findall(pattern, text)
    
    return results

def extract_quoted_content(text):
    # 正则表达式模式：匹配双引号内的内容
    # 针对 var locationDesc = "..." 这种格式
    pattern = r'var locationDesc = "(.*?)";'
    
    # 查找匹配的内容
    match = re.search(pattern, text)
    
    if match:
        # 返回第一个捕获组（即引号内的内容）
        return match.group(1)
    else:
        return "未找到匹配的内容"




def extract_ori_content(text):
    # 正则表达式模式：匹配双引号内的内容
    # 针对 var locationDesc = "..." 这种格式
    pattern = r"""<td headers="tb_02" class="tbg_6">　廠商地址</td>
					<td headers="tb_02" class="tbg_4R">
						<div class="tbc1L">(.*?)<span class="hardword">"""
    
    # 查找匹配的内容
    match = re.search(pattern, text)
    
    if match:
        # 返回第一个捕获组（即引号内的内容）
        return match.group(1)
    else:
        return "未找到匹配的内容"

# 88 - 114 
# years = list(range(114, 87, -1))
# for year in years:
#     url = f"https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&tenderStatusType=%E6%B1%BA%E6%A8%99&sortCol=AWARD_NOTICE_DATE&pageSize=100&timeRange={year}"
#     result = get_url_data_with_requests(url)
#     htmldata = result["data"]
#     num_pages = extract_numbers(htmldata)[-1]
#     print(f"current year: {year}, num of pages: {num_pages}")
#     for page_idx in range(1, num_pages+1):
#         page_url = f"https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?sortCol=AWARD_NOTICE_DATE&d-3611040-p={page_idx}&pageSize=100&tenderStatusType=%E6%B1%BA%E6%A8%99&querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&timeRange={year}"
#         page_data = get_url_data_with_requests(page_url)["data"]  
#         items = extract_href_content(page_data)
#         print(page_url)
#         print(items)
#         # for item_idx in range(0, len(items), 2):
#         #     item_url = "https://web.pcc.gov.tw/prkms/urlSelector/common/atm"+items[item_idx]
#         #     item_data = get_url_data_with_requests(item_url)["data"] 
#     break 

 
with open("taiwanorides.csv", 'a') as fp:
    session = requests.Session()
    years = list(range(113, 112, -1))
    for year in years:
        url = f"https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&tenderStatusType=%E6%B1%BA%E6%A8%99&sortCol=AWARD_NOTICE_DATE&pageSize=100&timeRange={year}"
        htmldata = session.get(url).text 
        num_pages = extract_numbers(htmldata)[-1]
        print("Cookies:")
        for key, value in session.cookies.items():
            print(f"{key}: {value}")
        print(f"current year: {year}, num of pages: {num_pages}")
        for page_idx in range(1, num_pages+1):
            if (year == 114 and page_idx == 1) or (year == 114 and page_idx == 2): 
                continue # TODO 

            time.sleep(2)
            page_url = f"https://web.pcc.gov.tw/prkms/tender/common/bulletion/readBulletion?sortCol=AWARD_NOTICE_DATE&d-3611040-p={page_idx}&pageSize=100&tenderStatusType=%E6%B1%BA%E6%A8%99&querySentence=%E9%86%AB%E8%97%A5%E7%94%A2%E5%93%81&timeRange={year}"
            _ = session.get(url).text 
            time.sleep(2)
            page_data = session.get(page_url).text  
            items = extract_href_content(page_data)
            # print(page_url)
            print(items)
            for item_idx in range(0, len(items), 2):
                time.sleep(15)
                item_url = "https://web.pcc.gov.tw/prkms/urlSelector/common/atm"+items[item_idx]
                print(item_url)
                # item_data = session.get(item_url).text 
                item_data = get_url_data_with_requests(item_url)["data"] # parall? 
                des_str = extract_quoted_content(item_data)
                print(des_str) 

                ori_str = ""
                soup = BeautifulSoup(item_data, 'html.parser')
                target_table = soup.find('table', class_='tb_06 sp2 tableShow')
                if target_table: 
                    tbc1L_divs = target_table.find_all('div', class_='tbc1L')
                    for divitem in tbc1L_divs: 
                        if "市" in divitem.text or "縣" in divitem.text:
                            ori_str = divitem.text 
                    if ori_str == "":
                        ori_str = "未找到指定hardword元素"
                else:
                    ori_str = "未找到指定类名的table元素"
                ori_str = ''.join(ori_str.split()) 
                print(ori_str) 
                fp.write(ori_str+","+des_str+"\n") 
                fp.flush()
            time.sleep(60*5)  

