jQuery( document ).ready(function( $ ) {
	var listUsers = window.pcidomain + "list-user.cgi"
	var getConversations = window.pcidomain + "conversations-with-user.cgi"
	var getFile = window.pcidomain + "history-for-file.cgi"
	
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

	$('select[name="user"]').change(function(){
		$.ajax({
			type: "GET",
			beforeSend: function(xhr){
				xhr.withCredentials = true
			},
			url: getConversations + "?target=" + $('select[name="user"]').val(),
			success: function(obj){
				if(obj.error){
					alert(obj.error)
					return
				}
				$('#history-list').html("") //Clear out the 'choose a user'
				obj.conversations = obj.conversations.sort()
				for (var i = obj.conversations.length - 1; i >= 0; i--) {
					var cName = obj.conversations[i]
					$('#history-list').append($("<li>"+cName+"</li>"))
				};

			}
		})
	})
	$('#history-list').on('click', 'li', function(){
		$.ajax({
			type: "GET",
			beforeSend: function(xhr){
				xhr.withCredentials = true
			},
			url: getFile + "?target=" + $('select[name="user"]').val() + "&f=" + $(this).text(),
			success: function(obj){
				$('textarea').html( obj.text.replace(/<br\/>/g,"\n") )
			}
		})
	})
})