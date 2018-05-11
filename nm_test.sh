#make
make ./ft_nm

#test
echo "[FLAG DIFFS]"
./ft_nm -a ./ft_nm > tu && nm -a ./ft_nm > ta && diff tu ta
./ft_nm -g ./ft_nm > tu && nm -g ./ft_nm > ta && diff tu ta
./ft_nm -p ./ft_nm > tu && nm -p ./ft_nm > ta && diff tu ta
./ft_nm -u ./ft_nm > tu && nm -u ./ft_nm > ta && diff tu ta
./ft_nm -U ./ft_nm > tu && nm -U ./ft_nm > ta && diff tu ta
./ft_nm -j ./ft_nm > tu && nm -j ./ft_nm > ta && diff tu ta
./ft_nm -r ./ft_nm > tu && nm -r ./ft_nm > ta && diff tu ta
./ft_nm -n ./ft_nm > tu && nm -n ./ft_nm > ta && diff tu ta

./ft_nm -a -r -j -U ./ft_nm > tu && nm -a -r -j -U ./ft_nm > ta && diff tu ta
./ft_nm -r -n ./ft_nm > tu && nm -r -n ./ft_nm > ta && diff tu ta

#cleanup
rm tu ta
