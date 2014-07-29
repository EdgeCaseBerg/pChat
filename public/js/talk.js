jQuery( document ).ready(function( $ ) {
	var listUsers = window.pcidomain + "list-user.cgi"

	$.ajax({
		type: "GET",
		beforeSend: function(xhr){
			xhr.withCredentials = true
		},
		url: listUsers,
		success: function(users){
			for (var i = users.length - 1; i >= 0; i--) {
				var user = users[i]
				$('select[name="user"]').append("<option value="+'"'+ user+ '"' + ">" + user + "</option>" )	
			};
			
		}
	})
})