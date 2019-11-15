<!DOCTYPE html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>N-Puzzle</title>
  <link rel="stylesheet" href="css/bulma.css">
  <link rel="stylesheet" href="css/style.css">
  <link rel="icon" href="css/icon.png" />
</head>
<body>
  <section class="hero is-light is-fullheight">
    <div class="hero-head"><a class="navbar-item" href="index.php">
      <h1 class="title">N-Puzzle</h1>
    </a></div>
      <div class="hero-body">
        <div class="container has-text-centered">
          <div class="column is-4 is-offset-4">
              <div id="container" class="is-invisible box"></div>
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
	function launch(elem) {
		elem.classList.add('is-loading');
        var data = <?php echo json_encode(openFile()) ?>;
        var container = document.getElementById("container");
        if (data == null) {
            container.classList.remove('is-invisible');
            container.innerHTML = "Désolé, il n'y a pas de donnée pour le moment.";
		    elem.classList.remove('is-loading');
            return ;
        }

        /*if (data.)
        var size = data;
        //document.getElementById("try").innerHTML = data;
        var container = document.getElementById("container");
        var table = document.createElement("table");
        table.classList.add('table');
        for ()*/

        console.log(data);
	}
</script>
