function cgi_fetch(form) {
	let selection = form.elements["yes"].checked;
	let post_body = "yes_no=" + selection;
	let action = form.action;
	if (selection === true) {
		const cgi_request = new Request(action, {method: 'POST', body: post_body});
		fetch(cgi_request)
		.then(data => {
			console.log("Perfect!");
			console.log(selection);
		});
	}
}

function sendForm(event) {
	const form = document.getElementById("session-form");
	event.preventDefault();
	cgi_fetch(form);
	form.style.display = "none";
}

document.addEventListener("DOMContentLoaded", () => {
	const form = document.getElementById("session-form");
	
	form.addEventListener("submit", function (event) {
		sendForm(event);
	});
});
