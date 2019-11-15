<?php
	function openFile() {
		$file = fopen("data", "r");
		$i = 0;
		while (($buf = fgets($file, 4096)) !== false) {
			$datas[$i++] = explode(" ", $buf);
		}
		fclose($file);
		return $datas;
	}

	require('visu.php');
?>
