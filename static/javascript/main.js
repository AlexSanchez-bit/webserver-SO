let sorttype = 0;

window.onload = () => {
  sortTable(sorttype);
}

function sortTable(columnIndex) {
  var table, rows, switching, i, x, y, shouldSwitch;
  table = document.getElementById("mytable");
  switching = true;

  while (switching) {
    switching = false;
    rows = table.rows;

    for (i = 0; i < (rows.length - 1); i++) {
      shouldSwitch = false;

      x = rows[i].getElementsByTagName("TD")[columnIndex];
      y = rows[i + 1].getElementsByTagName("TD")[columnIndex];

      if (isHigher(x.innerHTML.toLowerCase(), y.innerHTML.toLowerCase(), columnIndex)) {
        shouldSwitch = true;
        break;
      }
    }

    if (shouldSwitch) {
      rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
      switching = true;
    }
  }
}

function isHigher(el1, el2, comp_type) {
  switch (comp_type) {
    case 0:
      return el1 > el2;
    case 1:

      let d1 = new Date(el1);
      let d2 = new Date(el2);
      return d1 > d2;
    case 2:

      return parseInt(el1) * multipliyer(el1) > parseInt(el2) * multipliyer(el2);
  }
}

function multipliyer(el) {
  switch (el[el.length - 2]) {
    case 'K':
      return 100;
    case 'M':
      return 1000;
    case 'G':
      return 10000;
    default:
      return 1;

  }
}

var sortDropdown = document.getElementById("sortDropdown");
var selectedMethod = document.getElementById("selectedMethod");

sortDropdown.addEventListener("change", function() {
  var method = sortDropdown.value;
  sortTable(parseInt(method));
});
