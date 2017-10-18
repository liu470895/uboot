echo "输入你的源文件路径(默认从当前脚本文件的目录为根目录)："
ubuntu=ubuntu
ip_addr=192.168.11.128 #read ip_addr
read source_path
echo "源文件路径输入ok！"
echo "输入拷贝文件的到服务器中的目标位置:"
read dest_path
scp $source_path $ubuntu@$ip_addr:$dest_path
echo "传输ok!"
sleep 1

