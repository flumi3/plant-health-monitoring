import requests

response = requests.get("http://localhost:8000/devices")
print(response)
