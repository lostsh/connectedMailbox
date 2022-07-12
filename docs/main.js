
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

function showLast(anchor, letters){
    anchor.textContent = letters[0].created_at;
}