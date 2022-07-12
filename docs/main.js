
function main() {
    fetch('https://api.github.com/repos/lostsh/connectedMailbox/actions/runs')
        .then(function (response) {
            return response.json();
        })
        .then(function (resp) {
            console.log(resp);
            showLast(document.getElementById('lastLetter'), resp.workflow_runs);
        });

}

/**
 * Get the last arduino workflow run
 * from the github api data.
 * @param {*} anchor the anchor to display date
 * @param {*} letters array containing all runs
 */
function showLast(anchor, letters){

    var lastLetter = "";

    lastLetter = letters.find(element => element.name = "Arduino workflow");

    anchor.textContent = new Date(lastLetter.created_at).toLocaleString();
}