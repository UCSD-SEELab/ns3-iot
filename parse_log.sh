# run in ns-3 top directory after running waf

DIRECTORY="rel_logs"
m_script="read_and_plot.m"
node_count=2

if [ -d "$DIRECTORY" ]; then
	rm -rf $DIRECTORY
fi

mkdir $DIRECTORY

for node in $( echo 0 && echo "`seq $node_count`" )
do
	grep "NID=$node" power.log > $DIRECTORY/pow_node$node.log
	grep "NID=$node" temperature.log > $DIRECTORY/temp_node$node.log
	grep "NID=$node" reliability.log > $DIRECTORY/rel_node$node.log
done

### Create script file
if [ -f "$m_script" ]; then
	rm $m_script
fi

for node in $( echo 0 && echo "`seq $node_count`" )
do
	echo "pow_node$node = dlmread ('pow_node$node.log', ' ' , 0, 0);" >> $m_script
	echo "hf = figure" >> $m_script
	echo "plot(pow_node$node(:,1), pow_node$node(:,3));" >> $m_script
	echo "xlabel (\"time\");" >> $m_script
	echo "ylabel (\"Node$node Power\");" >> $m_script
	echo "print (hf, \"Node$node Power.jpg\", \"-djpg\");" >> $m_script
	echo >> $m_script

	echo "temp_node$node = dlmread ('temp_node$node.log', ' ' , 0, 0);" >> $m_script
	echo "hf = figure" >> $m_script
	echo "plot(temp_node$node(:,1), temp_node$node(:,3));" >> $m_script
	echo "xlabel (\"time\");" >> $m_script
	echo "ylabel (\"Node$node Temperature\");" >> $m_script
	echo "print (hf, \"Node$node Temperature.jpg\", \"-djpg\");" >> $m_script
	echo >> $m_script

	echo "rel_node$node = dlmread ('rel_node$node.log', ' ' , 0, 0);" >> $m_script
	echo "hf = figure" >> $m_script
	echo "plot(rel_node$node(:,1), rel_node$node(:,3));" >> $m_script
	echo "xlabel (\"time\");" >> $m_script
	echo "ylabel (\"Node$node Reliability\");" >> $m_script
	echo "print (hf, \"Node$node Reliability.jpg\", \"-djpg\");" >> $m_script
	echo >> $m_script
done

mv $m_script $DIRECTORY

