echo "输入你的下载的源文件路径(绝对路径)"
ubuntu=ubuntu
ip_addr=192.168.11.128 #read ip_addr
read source_path
echo "源文件路径输入ok！"
echo "输入拷贝的文件存放到本地的目标位置:"
read dest_path
scp $ubuntu@$ip_addr:$source_path $dest_path
echo "传输ok!"
sleep 1
