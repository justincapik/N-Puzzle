<!DOCTYPE html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>N-Puzzle</title>
  <link rel="stylesheet" href="css/bulma.css">
  <link rel="stylesheet" href="css/style.css">
  <link rel="icon" href="css/puzzle.png" />
</head>
<body>
  <section class="hero is-light is-fullheight">
    <div class="hero-head"><a class="navbar-item" href="index.php">
      <h1 class="title">N-Puzzle</h1>
    </a></div>
      <div class="hero-body">
        <div class="container has-text-centered">
          <div class="column is-4 is-offset-4">
              <div id="box" class="is-invisible box"></div>
			  <button onclick="launch(this)" class="button is-primary is-large">
	            Lancer
	          </button>
          </div>
        </div>
      </div>
  	<div class="hero-foot">
		<footer class="footer">
		  <div class="content has-text-centered">
		    <p>
		      By <a class="has-text-primary" href="https://profile.intra.42.fr/users/lmartin-">lmartin-</a>
		    </p>
		  </div>
		</footer>
	</div>
  </section>
</body>

<script>

	function sleep(ms) {
	  return new Promise(resolve => setTimeout(resolve, ms));
	}

	async function launch(elem) {
		elem.classList.add('is-loading');
        var data = <?php echo json_encode(openFile()) ?>;
        var box = document.getElementById("box");
        if (data == null) {
            box.classList.remove('is-invisible');
            box.innerHTML = "Désolé, il n'y a pas de donnée pour le moment.";
		    elem.classList.remove('is-loading');
            return ;
        }

		var size = data.length;
        var table = document.createElement("table");
		table.classList.add('table');
		table.classList.add('is-bordered');
        table.classList.add('margin-auto');

		box.innerHTML = "";
		box.appendChild(table);
		box.classList.remove('is-invisible');
        for (var i = 0; i < size; i++) {
			console.log(i);


			createRows(data[i], table, data[size - 1]);
			await sleep(1000);
		}
		elem.classList.remove('is-loading');


	}

	function createRows(data, table, soluce) {
		var size = Math.sqrt(data.length);
		table.innerHTML = "";
		var nb = 0;
		for (var i = 0; i < size; i++) {
			var row = document.createElement("tr");
			for (var n = 0; n < size; n++) {
				var cell = document.createElement("td");
				if (data[nb] == soluce[nb])
					cell.classList.add('is-primary');

				cell.appendChild(document.createTextNode(data[nb++]));
				row.appendChild(cell);
			}
			table.appendChild(row);
			console.log(table);
		}
	}
</script>
