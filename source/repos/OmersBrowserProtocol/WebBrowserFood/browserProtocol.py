import socket
import requests
import re
import os
import time

PORT = 90
IP = "127.0.0.1"
BUFFER_MAX = 2048


def main():
    listening_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('', PORT)
    listening_socket.bind(server_address)
    listening_socket.listen(1)
    while True:
        try:
            pattern = r'([a-zA-Z0-9._-]+):\s*(https?://[^\s<>]+)'
            response = requests.get("https://raw.githubusercontent.com/Frogithezhrda/Saban-Web/main/DNS")
            matches = re.findall(pattern, response.text)
            domain_list = {}
            print(matches)
            for match in matches:
                domain_list[match[0].strip()] = match[1].strip()
            client_soc, client_address = listening_socket.accept()
            full_msg = client_soc.recv(BUFFER_MAX).decode()
            if "VIDEO" in full_msg:
                image_url = full_msg[6:]
                if "github.com" in image_url:
                    image_url = image_url.replace("github.com", "raw.githubusercontent.com")
                    image_url = image_url.replace("/blob", "")
                    image_name = image_url.rsplit('/', 1)[-1]
                    print(image_url)
                    img_data = requests.get(image_url).content
                    with open(image_name, 'wb') as handler:
                        handler.write(img_data)
                    print(image_name)
                client_soc.sendall(image_name.encode())
            if "IMAGE" in full_msg:
                image_url = full_msg[6:]
                if "github.com" in image_url:
                    image_url = image_url.replace("github.com", "raw.githubusercontent.com")
                    image_url = image_url.replace("/blob", "")
                print(image_url)
                img_data = requests.get(image_url).content
                with open('temp.jpg', 'wb') as handler:
                    handler.write(img_data)
                client_soc.sendall("IMAGE SENDED".encode())
            else:
                full_msg = full_msg.lower()
                if full_msg in domain_list.keys():
                    full_msg = domain_list[full_msg]
                    full_msg = full_msg.replace("github.com", "raw.githubusercontent.com")
                    full_msg = full_msg.replace("/blob", "")
                    response = requests.get(full_msg)
                    response = response.text
                    client_soc.sendall(response.encode())
                else:
                    full_msg = "<title>ERROR 404\n<h1>Error 404 No Such Domain:("
                    client_soc.sendall(full_msg.encode())
        except Exception:
            full_msg = "<title>ERROR 404\n<h1>No Internet"
            client_soc.sendall(full_msg.encode())
            print("Exception")
            time.sleep(3)
    listening_socket.close()


if __name__ == '__main__':
    main()
