import requests
TOKEN = "6233250501:AAFuVuQdSnJul5rW2N7KKskDbMqVXMApj_s"
url = f"https://api.telegram.org/bot{TOKEN}/getUpdates"
print(requests.get(url).json())
