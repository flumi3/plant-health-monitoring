with open("./src/firmware_version.cpp",'r+') as file:
    text = file.readline()
    text = text.strip(";")
    version_number = text.split("=")[1]
    new_version_nuber = int(version_number)+1
    file.seek(0,0)
    file.write(f'const char* firmware_version = "{new_version_nuber}";')

    