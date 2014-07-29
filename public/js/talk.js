jQuery( document ).ready(function( $ ) {
	var listUsers = window.pcidomain + "list-user.cgi"
	var getConversation = window.pcidomain + "conversation.cgi"

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

	var period = 2000
	var timeout = null
	var user = null
	var lastChecked = new Date();
	var loadConversation = function(){
		$.ajax({
			type: "GET",
			beforeSend: function(xhr){
				xhr.withCredentials = true
			},
			url: getConversation + "?target="+$('select[name="user"]').val() + "&date=" + lastChecked.getTime(),
			success: function(response){
				console.info("Last conversation retrieved at: " + lastChecked)
				lastChecked = new Date()
				$('#history').html(response.text)
			}
		})

		timeout = setTimeout(loadConversation, period)
	}

	$('select').on('change', function(evt){
		if(timeout != null){
			clearTimeout(timeout)
		}
		timeout = setTimeout(loadConversation, period)
	})
})