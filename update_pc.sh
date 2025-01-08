#!/bin/bash

# Check for two arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <path to folder with .pc files> <new prefix>"
    exit 1
fi

# Arguments
PC_FILES_DIR="$1"
NEW_PREFIX="$2"

# Check if the specified folder exists
if [ ! -d "$PC_FILES_DIR" ]; then
    echo "Error: Specified folder '${PC_FILES_DIR}' does not exist."
    exit 1
fi

# Check if the second parameter is non-empty
if [ -z "$NEW_PREFIX" ]; then
    echo "Error: New prefix cannot be empty."
    exit 1
fi

# Get the absolute path of the specified directory
PC_FILES_DIR=$(realpath "$PC_FILES_DIR")

# Check for .pc files in the specified directory
shopt -s nullglob
pc_files=("$PC_FILES_DIR"/*.pc)
shopt -u nullglob

if [ ${#pc_files[@]} -eq 0 ]; then
    echo "No .pc files found in the specified folder '${PC_FILES_DIR}'."
    exit 0
fi

# Process each .pc file
for pc_file in "${pc_files[@]}"; do
    echo "Processing file: $pc_file"


    # Update prefix
    if grep -q "^\s*prefix\s*=" "$pc_file"; then
        sed -i "s|^\s*prefix\s*=.*$|prefix=${NEW_PREFIX}|g" "$pc_file"
        echo "prefix updated to: ${NEW_PREFIX}"
    else
        echo "prefix=${NEW_PREFIX}" | cat - "$pc_file" > temp && mv temp "$pc_file"
        echo "prefix added: ${NEW_PREFIX}"
    fi

    # Update libdir only if /lib is not already added
    if grep -q "libdir=\${exec_prefix}$" "$pc_file"; then
        sed -i "s|libdir=\${exec_prefix}$|libdir=\${exec_prefix}/lib|g" "$pc_file"
        echo "libdir updated for: \${exec_prefix}"
    elif grep -q "libdir=\${prefix}$" "$pc_file"; then
        sed -i "s|libdir=\${prefix}$|libdir=\${prefix}/lib|g" "$pc_file"
        echo "libdir updated for: \${prefix}"
    else
        echo "libdir already contains /lib, no changes required."
    fi

    # Update Name field for specific .pc files
    case "$(basename "$pc_file")" in
        "x265.pc")
	    if grep -q "^Name:" "$pc_file"; then
            sed -i 's/^Name:.*$/Name: x265/' "$pc_file"
            echo "Field 'Name' has been updated to 'x265' in file: $pc_file"
        else
            echo "Field 'Name' not found in the file. No changes made."
        fi

        if grep -q "^prefix=" "$pc_file"; then
            sed -i 's|^prefix=|# prefix=|g' "$pc_file"
            echo "Field 'prefix' has been commented out in file: $pc_file"
        else
            echo "Field 'prefix' not found in the file. No changes made."
        fi

	    #if grep -q "^Libs:" "$pc_file"; then
	    #    sed -i 's|^Libs:.*$|Libs: -L${libdir} -laom|' "$pc_file"
    	    #	echo "Field 'Libs' has been updated to '-L${libdir} -laom' in file: $pf_file"
 	    #else
    	    #	echo "Field 'Libs' not found in the file. No changes made."
	    #fi

            #if grep -q "^\s*Name\s*=" "$pc_file"; then
            #    sed -i "s|^\s*Name\s*=.*$|Name=x265|g" "$pc_file"
            #    echo "Name updated to: x265"
            #fi
            ;;
        "aom.pc")
	    if grep -q "^Name:" "$pc_file"; then
    		sed -i 's/^Name:.*$/Name: aom/' "$pc_file"
    		echo "Field 'Name' has been updated to 'aom' in file: $pc_file"
	    else
		echo "Field 'Name' not found in the file. No changes made."
	    fi

	    if grep -q "^Libs:" "$pc_file"; then
                sed -i 's|^Libs:.*$|Libs: -L${libdir} -laom|' "$pc_file"
                echo "Field 'Libs' has been updated to '-L${libdir} -laom' in file: $pc_file"
            else
                echo "Field 'Libs' not found in the file. No changes made."
            fi
            #if grep -q "^\s*Name\s*=" "$pc_file"; then
                #sed -i "s|^\s*Name\s*=.*$|Name=aom|g" "$pc_file"
                #echo "Name updated to: aom"
            #fi
            ;;
        *)
            echo "No Name update needed for this file."
            ;;
    esac

    echo "----------------------------------------"
done

echo "All .pc files in the folder '${PC_FILES_DIR}' have been successfully updated."


# #!/bin/bash

# # Проверка на наличие двух аргументов
# if [ $# -ne 2 ]; then
#     echo "Использование: $0 <путь к папке с .pc файлами> <новый prefix>"
#     exit 1
# fi

# # Аргументы
# PC_FILES_DIR="$1"
# NEW_PREFIX="$2"

# # Проверяем, существует ли указанная папка
# if [ ! -d "$PC_FILES_DIR" ]; then
#     echo "Ошибка: Указанная папка '${PC_FILES_DIR}' не существует."
#     exit 1
# fi

# # Проверяем, является ли второй параметр непустым
# if [ -z "$NEW_PREFIX" ]; then
#     echo "Ошибка: Новый prefix не может быть пустым."
#     exit 1
# fi

# # Получаем абсолютный путь к указанной директории
# PC_FILES_DIR=$(realpath "$PC_FILES_DIR")

# # Проверяем наличие .pc файлов в указанной директории
# shopt -s nullglob
# pc_files=("$PC_FILES_DIR"/*.pc)
# shopt -u nullglob

# if [ ${#pc_files[@]} -eq 0 ]; then
#     echo "В указанной папке '${PC_FILES_DIR}' нет .pc файлов для обработки."
#     exit 0
# fi

# # Проходим по каждому .pc файлу
# for pc_file in "${pc_files[@]}"; do
#     echo "Обрабатывается файл: $pc_file"

#     # Создаём резервную копию файла
#     cp "$pc_file" "${pc_file}.bak"

#     # Изменяем prefix
#     if grep -q "^\s*prefix\s*=" "$pc_file"; then
#         sed -i "s|^\s*prefix\s*=.*$|prefix=${NEW_PREFIX}|g" "$pc_file"
#         echo "prefix обновлён на: ${NEW_PREFIX}"
#     else
#         echo "prefix=${NEW_PREFIX}" | cat - "$pc_file" > temp && mv temp "$pc_file"
#         echo "prefix добавлен: ${NEW_PREFIX}"
#     fi

#     # Изменяем libdir только если не добавлено /lib
#     if grep -q "libdir=\${exec_prefix}$" "$pc_file"; then
#         sed -i "s|libdir=\${exec_prefix}$|libdir=\${exec_prefix}/lib|g" "$pc_file"
#         echo "libdir обновлён для: \${exec_prefix}"
#     elif grep -q "libdir=\${prefix}$" "$pc_file"; then
#         sed -i "s|libdir=\${prefix}$|libdir=\${prefix}/lib|g" "$pc_file"
#         echo "libdir обновлён для: \${prefix}"
#     else
#         echo "libdir уже содержит /lib, изменений не требуется."
#     fi

#     echo "Резервная копия создана: ${pc_file}.bak"
#     echo "----------------------------------------"
# done

# echo "Все .pc файлы в папке '${PC_FILES_DIR}' успешно обновлены."
